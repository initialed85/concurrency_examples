package main

import (
	"fmt"
	"log"
	"os"
	"os/signal"
	"sync"
	"syscall"
)

var (
	counter = 0
	lock    sync.Mutex
	stop    = false
	useLock = false
)

func work(identifier int) {
	if useLock {
		lock.Lock()
	}

	fmt.Printf("%v: work %v...\n", identifier, counter)
	for i := 0; i < 65536; i++ {
		j := i * 65536
		_ = j
	}
	counter++

	if useLock {
		lock.Unlock()
	}
}

func loop(identifier int) {
	for !stop {
		work(identifier)
	}
}

func startGoroutines() {
	for i := 0; i < 8; i++ {
		go loop(i)
	}
}

func waitForCtrlC() {
	sig := make(chan os.Signal, 2)
	signal.Notify(sig, os.Interrupt, syscall.SIGTERM)

	log.Printf("Press Ctrl + C to exit...")
	<-sig
}

func main() {
	log.Printf("%#+v", os.Args)
	if len(os.Args) == 1 {
		fmt.Printf("usage: %v [locked|unlocked]\n\nerror: first argument not given", os.Args[0])
		os.Exit(1)
		return
	}

	if len(os.Args) > 1 {
		if os.Args[1] == "locked" {
			useLock = true
		}
	}

	signals := make(chan os.Signal, 1)
	signal.Notify(signals, syscall.SIGINT)
	handler := func() {
		<-signals
		stop = true
	}
	go handler()

	startGoroutines()
	waitForCtrlC()
}
