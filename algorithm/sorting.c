void bubble_sort(int a[],int length){
    for(int i=0;i<length-1;i++){                                //遍历数组
        for(int j=0;j<length-i-1;j++){                          //两两比较，大的放到后面去，大的元素不断向后，小的元素不断向前，排序完成
            if(a[j]>a[j+1])
                a[j]^=a[j+1]^=a[j]^=a[j+1];                     //交换a[j]和a[j+1]的值
        }
    }
}

void select_sort(int a[],int length){
    for(int i=0;i<length-1;i++){                                //遍历数组
        int min_index=i;                                        //每次遍历默认第一个元素最小
        for(int j=i+1;j<length;j++){                            //选出最小的元素
            if(a[min_index]>a[j])
                min_index=j;
        }
        if(min_index!=i)                                        //如果第一个元素就是最小，跳过；否则把它和第一个元素交换
            a[min_index]^=a[i]^=a[min_index]^=a[i];
    }
}

void insert_sort(int a[],int length){
    int temp;
    for(int i=1;i<length;i++){
        temp=a[i];                                              //选定第一个元素
        int j;
        for(j=i;j>0&&a[j-1]>temp;j--)                           //把元素后移，直到空出数组头或者碰到比选定元素小的元素
            a[j]=a[j-1];
        a[j]=temp;                                              //把选定的元素插入空位
    }
}

void quick_sort(int a[],int length){
    if(length<2)                                                //短到不需要排序
        return;
    int i,j,p=a[length/2];                                      //p是选定的中间元素
    for(i=0,j=length-1;;i++,j--){                               //以p为中心，比p小的放到p前面，比p大的放到p后面
        while(a[i]<p)                                           //从前往后扫描，直到扫到p或者比p大的元素
            i++;
        while(a[j]>p)                                           //从后往前扫描，直到扫到p或者比p小的元素
            j--;
        if(i>=j)                                                //如果已经扫描到p或者超过扫描范围，跳过
            break;
        a[i]^=a[j]^=a[i]^=a[j];                                 //交换比p大和比p小的元素
    }
    quick_sort(a,i);                                            //给p的前半部分排序
    quick_sort(a+i,length-i);                                   //给p的后半部分排序
} 
