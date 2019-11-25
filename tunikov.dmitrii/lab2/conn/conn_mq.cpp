//
// Created by dmitrii on 23.11.2019.
//

#include <conn.h>
#include <message.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <mqueue.h>

int Conn::desc;
const char* Conn::channel_name;

Conn::Conn()
{
    channel_name = "/mq_queue";
}

bool Conn::Open(size_t id, bool create)
{
    bool res = false;
    is_host = create;
    int mqflg = O_RDWR;
    int mqperm = 0666;

    if (is_host)
    {
        std::cout << "Creating connection with id = " << id << std::endl;
        mqflg |= O_CREAT;
        struct mq_attr attr = ((struct mq_attr){0, 1, sizeof(Message), 0, {0}});
        desc = mq_open(channel_name, mqflg, mqperm, &attr);
    }
    else
    {
        std::cout << "Getting connection with id = " << id << std::endl;
        desc = mq_open(channel_name, mqflg);
    }

    if (desc == -1)
    {
        std::cout << "ERROR: mq_open failed, errno = " << strerror(errno) << std::endl;
    }
    else
    {
        std::cout << "mq_open returned id = " << desc << std::endl;
        res = true;
    }
    return res;
}

bool Conn::Read(void* buf, size_t count)
{
    Message mq_buf;
    bool success = true;
    if (mq_receive(desc, (char *)&mq_buf, sizeof(Message), nullptr) == -1)
    {
        std::cout << "ERROR: mq_recieve failed, errno = " << strerror(errno) << std::endl;
        success = false;
    }
    else
    {
        *((Message*) buf) = mq_buf;
    }
    return success;
}

bool Conn::Write(void* buf, size_t count)
{
    bool res = false;
    if (count <= sizeof(Message))
    {
        if (mq_send(desc, (char*)buf, count, 0) == -1)
        {
            std::cout << "ERROR: mq_send failed, errno = " << strerror(errno) << std::endl;
        }
        else
        {
            res = true;
        }
    }
    return res;
}

bool Conn::Close()
{
    bool res = false;
    if (mq_close(desc) == 0)
    {
        if (!is_host)
        {
            res = true;
        }
        else if (is_host && mq_unlink(channel_name) == 0)
        {
            res = true;
        }
    }
    return res;
}


