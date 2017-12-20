int CLocalListener::start_local_service()
{

    int ret = -1;
    ObjectFd object_fd;//封装的fd结构体
    struct sockaddr_un serv_addr;
    fsock_info local_watch_sock[MAXSOCKFD];

    memset(local_watch_sock, 0, sizeof(local_watch_sock));  
    ser_sock_fd = _sock_mgr.sock_create(AF_UNIX, SOCK_STREAM, 0);//创建本地套接字
    if (ser_sock_fd == -1)
    {
        perror("socket failed!");
        return NAZ_ERR_SOCK_ERROR;
    }

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    unlink(UNIX_DOMAIN);
    memcpy(serv_addr.sun_path, UNIX_DOMAIN, sizeof(serv_addr.sun_path) - 1);//设置绑定所需的参数
    ret = _sock_mgr.sock_bind(ser_sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));//绑定套接字
    if (ret == -1)
    {
        perror("bind failed!");
        close(ser_sock_fd);
        return NAZ_ERR_SOCK_ERROR;
    }
    _sock_mgr.sock_listen(ser_sock_fd, MAXSOCKFD);//监听套接字

    object_fd.watch_sock = local_watch_sock;
    object_fd._sock_mgr = _sock_mgr;
    //创建接收线程，满足多个请求连接到该套接字的情况
    if (pthread_create(&recv_thread, NULL, _sock_mgr.s_recv_routine, (void *)&object_fd) != 0)
    {
        perror("pthread_create failed:\n");
        close(ser_sock_fd);
        return (-1);
    }
    //accept，将accept的套接字更新到local_watch_sock数组中，由上边的recv线程收取并采取相应的操作
    _sock_mgr.accept_routine(ser_sock_fd, local_watch_sock, 0);
    pthread_join(recv_thread, NULL);

    if (ser_sock_fd > 0)
    {
        close(ser_sock_fd);
    }

    return 0;
}
