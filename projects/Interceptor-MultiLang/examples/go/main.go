package main

import (
	interceptor "github.com/TitanCSA/Interceptor-MultiLang/bindings/go"
	"log"
)

func main() {
	log.Print("Invert mouse Y. Close the program to restore normal input.")
	io, err := interceptor.New()
	if err != nil {
		log.Fatal(err)
	}
	defer io.Close()
	for {
		inputs, err := io.Receive(-1)
		if err != nil {
			log.Print(err)
			return
		}
		for _, input := range inputs.Inputs {
			if input.Kind == "mouse" && input.State["flags"]&1 == 0 {
				input.State["y"] = -input.State["y"]
			}
		}
		if _, err = io.Send(inputs); err != nil {
			log.Print(err)
			return
		}
	}
}
