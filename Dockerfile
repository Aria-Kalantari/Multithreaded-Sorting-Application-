FROM gcc:latest

WORKDIR /app

COPY multithread_sort.c .

RUN gcc -o multithread_sort multithread_sort.c -pthread

CMD ["./multithread_sort"]
