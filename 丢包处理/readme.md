# 丢包的处理方法

## 发送端

### 相关结构

首先新建一个当前帧和上一帧的结构体

```c
typedef struct data
{
    uint8_t data[5];	//传输的数据
    uint8_t seq;		//当前序列
    uint8_t old_data[5];
    uint8_t old_seq;
}frame_data;

typedef frame_data * frame_handle;
```

写好更新结构体的结构

```c
void refresh(frame_handle frame){
   
    for (int i = 0; i < 4; i++)
    {
        frame->old_data[i] = frame->data[i];
        frame->data[i]++;
    }
    frame->old_seq = frame->seq;
    frame->old_data[4] = frame->old_seq;
    frame->seq++;
    frame->data[4] = frame->seq;
}

```



### 处理逻辑

1. 发送端口先发一帧数据，然后持续检测收到的数据
2. 当没有收到数据超时情况，重新发送一帧数据
3. 当收到数据时候，分为三种情况
   1. 收到的数据是对的： 发送下一帧数据，更新结构体
   2. 收到的数据不对，序列号相差一帧（说明接收端没有接受到）：重新发送当前帧的数据
   3. 收到的数据不对，序列号相差两帧（说明断开连接了，要重新链接）：重置当前序列号以及数据，重新发送







## 接收端

接收端只负责处理数据，以及错误重发



### 处理逻辑

1. 没有收到数据，进入阻塞状态（没必要探测，会产生大量功耗）
2. 收到数据，分为两种情况
   1. 收到的数据是对的，处理收到的数据，发送下一个序列
   2. 收到的数据不对，单独发送下一个序列即可



## 发送接收过程

因为接收端一直等待接收数据，没有数据就进入阻塞状态，所以这个模型一定是发送短来发起链接



1. 发送端发出数据（正常发或者接收数据超时两种情况都发送）
2. 接收端检测收到的序列，查看是否是想要的序列，是的话就处理数据，发送想要的下一帧序列，不是重新发送这一帧序列
3. 发送端接收到序列，与自己的序列进行检测，如果序列相同，即可发送下一帧数据
4. 如果序列不同，检测序列数相差多少，如果仅仅相差一帧，表示接收端丢包了，重新发送上一帧数据。如果相差大于一帧，代表接收端或者发送端有一个断开连接了，需要重置当前的序列号以及数据





## 关键代码(上面描述的举例)：

### 发送端(截取一段)：

```c
	Ebyte_Send( &my_status, frame1.data, pingLength, 0) ;
    for (int i = 0; i < pingLength; i++)
    {
        printf("%d, ",frame1.data[i]);
    }
    printf("\n");
    refresh(&frame1);
    uint8_t rec_seq[100] = {1,1,1,1,1};
    printf("frame seq = %d\n",frame1.seq);
    printf("data seq = %d\n",frame1.data[4]);

    while(1){
        Ebyte_Receive(&my_status, rec_seq, 1000);
        printf("seq  is %d\n",rec_seq[0]);
       // printf("frame 1 seq  is %d\n",frame1.seq);
        if (abs(rec_seq[0] - frame1.old_data[4]) > 1)
        {
            printf("step 1\n");
            frame1.seq = rec_seq[0];
            frame1.data[4] = rec_seq[0];
        }
        
        if (frame1.seq == rec_seq[0])
        {
            //接收正确
            Ebyte_Send( &my_status, frame1.data, pingLength, 0 );
            for (int i = 0; i < pingLength; i++)
            {
                printf("%d, ",frame1.data[i]);
            }
            printf("\n");
            refresh(&frame1);
            // printf("frame seq = %d\n",frame1.seq);
            // printf("data seq = %d\n",frame1.data[4]);
        }else
        {
            //接收错误
            printf("send error\n");
             for (int i = 0; i < pingLength; i++)
            {
                printf("%d, ",frame1.old_data[i]);
            }
            printf("\n");
            Ebyte_Send( &my_status, frame1.old_data, pingLength, 0 );
        }
    
```



### 接收端：

```c
    while(1){
        Ebyte_Receive( &my_status, rev_data, 0 );
        
        // ets_delay_us(100);
        if (rec_seq == rev_data[4])
        {
             //接收正确       
            printf("seq is %d \n",rev_data[4]);
            for (int i = 0; i < 4; i++)
            {
                printf("%d ",rev_data[i]);
            }
            printf("\n");
            rec_seq++;
            Ebyte_Send(&my_status, &rec_seq, 1,0);
            
        }else
        {
            //接收错误
           // printf("rec error\n");
            Ebyte_Send(&my_status, &rec_seq, 1,0);

        }
        
        ets_delay_us(500);

    }

```





## 结果

### 发送端

![image-20230704173949788](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230704173949788.png)

### 接收端



![image-20230704174002107](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230704174002107.png)





解决了丢包了问题