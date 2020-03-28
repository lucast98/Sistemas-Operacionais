int main (){
    for (volatile unsigned long long i = 0; i < 1000000000ULL; ++i); //muitas iterações; ULL = unsigned long long
    
    return 0;
}