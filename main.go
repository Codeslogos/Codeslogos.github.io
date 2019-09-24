package main

import (
	"fmt"
	"strings"

	"github.com/nlopes/slack"
)

type order struct {
	Name     string
	Size     string
	Quantity string
}

type orders []order

type menu map[string]bool

type sizes map[string]bool

func main() {
	api := slack.New(
		"xoxb-768970633397-757496170067-rNVlPwHEWYH5i7sGYoMy5q3x",
	)

	var PeppesMenu menu = menu{
		"pepperoni": true,
		"margarita": true,
		"italian":   false,
		"lepyoshka": true,
	}

	var PizzaSizes sizes = sizes{
		"small":  true,
		"normal": false,
		"big":    true,
	}

	ordFlag := false
	adrsFlag := false
	var ords orders
	var adrs string
	OrdNum := 0

	rtm := api.NewRTM()
	go rtm.ManageConnection()

	for msg := range rtm.IncomingEvents {
		fmt.Print("Event Received: ")
		switch ev := msg.Data.(type) {
		case *slack.HelloEvent:
			rtm.SendMessage(rtm.NewOutgoingMessage("Если хотите сделать заказ напишите <ЗАКАЗ>", "DNLET6812"))

		case *slack.ConnectedEvent:
			fmt.Println("Infos:", ev.Info)
			fmt.Println("Connection counter:", ev.ConnectionCount)

		case *slack.MessageEvent:
			fmt.Printf("Message: %v\n", ev)
			text := ev.Text
			if text == "ЗАКАЗ" && ordFlag == false {
				rtm.SendMessage(rtm.NewOutgoingMessage(`Пожалуйста, составьте заказ в формате: <наименование товара + размер/объём + количество>(вместо + пробел). Как только закажете всё необходимое, пожалуйста, напишите Peppe`, "DNLET6812"))
				rtm.SendMessage(rtm.NewOutgoingMessage(`В данный момент в нашем меню есть следующие варианты пиццы:`, "DNLET6812"))
				for s, b := range PeppesMenu {
					if b {
						rtm.SendMessage(rtm.NewOutgoingMessage(s, "DNLET6812"))
					}
				}
				rtm.SendMessage(rtm.NewOutgoingMessage(`А также следующие размеры:`, "DNLET6812"))
				for s, b := range PizzaSizes {
					if b {
						rtm.SendMessage(rtm.NewOutgoingMessage(s, "DNLET6812"))
					}
				}
				rtm.SendMessage(rtm.NewOutgoingMessage("Слушаю ваш заказ", "DNLET6812"))
				ordFlag = true
				break
			}
			if ordFlag == true && text == "Peppe" {

				if OrdNum != 0 {
					rtm.SendMessage(rtm.NewOutgoingMessage("Укажите адрес доставки", "DNLET6812"))
					adrsFlag = true
					break
				}

			}
			if ordFlag && adrsFlag {
				rtm.SendMessage(rtm.NewOutgoingMessage("Ваш заказ:", "DNLET6812"))
				for _, ord := range ords {
					rtm.SendMessage(rtm.NewOutgoingMessage(ord.Name+" "+ord.Size+" "+ord.Quantity, "DNLET6812"))
					adrs = text
					rtm.SendMessage(rtm.NewOutgoingMessage("На адрес "+adrs, "DNLET6812"))
					rtm.SendMessage(rtm.NewOutgoingMessage("принят", "DNLET6812"))
				}
			}
			if ordFlag && !adrsFlag {
				s := strings.Fields(text)
				if len(s) == 3 {

					var ord order
					if PeppesMenu[s[0]] {
						ord.Name = s[0]
					} else {
						rtm.SendMessage(rtm.NewOutgoingMessage("К сожалению в данный момент такого - <"+s[0]+"> нашем меню нет. Попробуйте заказать что-нибудь другое.", "DNLET6812"))
						break
					}
					if PizzaSizes[s[1]] {
						ord.Size = s[1]
					} else {
						rtm.SendMessage(rtm.NewOutgoingMessage("К сожалению в данный момент такого - <"+s[1]+"> размера данной - <"+s[0]+"> пиццы в нашем меню нет. Попробуйте заказать что-нибудь другое.", "DNLET6812"))
						break
					}
					ord.Quantity = s[2]
					ords = append(ords, ord)
					OrdNum++
				} else {
					rtm.SendMessage(rtm.NewOutgoingMessage("Неверный формат ввода. Повторите заказ.", "DNLET6812"))
				}

			}

			fmt.Printf("%s\n", text)

		case *slack.PresenceChangeEvent:
			fmt.Printf("Presence Change: %v\n", ev)

		case *slack.LatencyReport:
			fmt.Printf("Current latency: %v\n", ev.Value)

		case *slack.RTMError:
			fmt.Printf("Error: %s\n", ev.Error())

		case *slack.InvalidAuthEvent:
			fmt.Printf("Invalid credentials")
			return

		default:

			// Ignore other events..
			// fmt.Printf("Unexpected: %v\n", msg.Data)
		}
	}
}
