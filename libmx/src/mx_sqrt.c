int mx_sqrt(int x) {
    double curr = 0;
    double next = x;
    double check = x;
    while (check >= 1) {
        curr = next;
        next = (curr + x / curr) / 2;
        check = next - curr;
        check = check > 0 ? check : -check;
    }
    return (int) curr * (int) curr == x ? curr : 0;
}
