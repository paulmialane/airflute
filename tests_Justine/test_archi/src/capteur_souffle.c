int n = 0;

int souffle_yes_no(void){
    n = n + 1;
    if (n%2 == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int souffle_force(void){
    return 127;
}