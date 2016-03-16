// #include <cstddef> // std::size_t

template <class T,std::size_t N> void bubble_sort(T (&a)[N]){
    for(auto i=0UL;i<N-1;i++){                                //遍历数组
        for(auto j=0UL;j<N-i-1;j++){                          //两两比较，大的放到后面去，大的元素不断向后，小的元素不断向前，排序完成
            if(a[j]>a[j+1])
                a[j]^=a[j+1]^=a[j]^=a[j+1];                     //交换a[j]和a[j+1]的值
        }
    }
}

template <class T,std::size_t N> void select_sort(T (&a)[N]){
    for(auto i=0UL;i<N-1;i++){                                //遍历数组
        auto min_index=i;                                        //每次遍历默认第一个元素最小
        for(auto j=i+1;j<N;j++){                            //选出最小的元素
            if(a[min_index]>a[j])
                min_index=j;
        }
        if(min_index!=i)                                        //如果第一个元素就是最小，跳过；否则把它和第一个元素交换
            a[min_index]^=a[i]^=a[min_index]^=a[i];
    }
}

template <class T,std::size_t N> void insert_sort(T (&a)[N]){
    auto temp;
    for(auto i=1UL;i<N;i++){
        temp=a[i];                                              //选定第一个元素
        auto j;
        for(j=i;j>0UL&&a[j-1]>temp;j--)                           //把元素后移，直到空出数组头或者碰到比选定元素小的元素
            a[j]=a[j-1];
        a[j]=temp;                                              //把选定的元素插入空位
    }
}

template <class T> void quick_sort(T a[],std::size_t length){
    if(length<2)                                                //短到不需要排序
        return;
    std::size_t i,j,pivot=a[length/2];                                      //pivot是选定的中间元素
    for(i=0,j=length-1;;i++,j--){                               //以pivot为中心，比pivot小的放到pivot前面，比pivot大的放到pivot后面
        while(a[i]<pivot)                                           //从前往后扫描，直到扫到pivot或者比pivot大的元素
            i++;
        while(a[j]>pivot)                                           //从后往前扫描，直到扫到pivot或者比pivot小的元素
            j--;
        if(i>=j)                                                //如果已经扫描到pivot或者超过扫描范围，跳过
            break;
        a[i]^=a[j]^=a[i]^=a[j];                                 //交换比pivot大和比pivot小的元素
    }
    quick_sort(a,i);                                            //给p的前半部分排序
    quick_sort(a+i,length-i);                                   //给p的后半部分排序
} 
