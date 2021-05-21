# concurrency_examples

## Python

```
virtualenv -p $(which python3) venv
venv/bin/python main.py [thread|process]
```

## C

```
gcc -o c main.c && ./c [thread|process]
```

## Go

```
go build -o main main.go && ./main [locked|unlocked]
```
