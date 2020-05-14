char* decToBin(int n,int no){
    char *bin = (char *)malloc(no+1);
    int i;
    for(i = no-1; i >= 0; --i){
        bin[i] = ('0' + n%2);
        n /= 2;    
    }
    bin[no] = '\0';
    return bin;
}
      
int binToDec(char *bin,int no){
    static int dec = 0;
    int i;
    for(i = 0; i < no; ++i){
        dec = dec + (ex2(i) * (bin[9-i]-'0'));
    }
    return dec;
} 
