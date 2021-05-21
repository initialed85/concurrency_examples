# concurrency_examples

## Python

```
virtualenv -p $(which python3) venv
venv/bin/python main.py [thread|process]
```

### Observations

- thread = 1 process capped at about 100% of a core
- process = 8 processes all capped at 100% of a core (unless there are less than 8 cores)

## C

```
gcc -o c main.c && ./c [thread|process]
```

### Observations

- thread = 1 process using all cores
- process = 8 processes spread across all cores

## Go

```
go build -o main main.go && ./main [locked|unlocked]
```

### Observations

- locked = 1 process using approximately 1 core
- unlocked = 1 process spread across all cores
