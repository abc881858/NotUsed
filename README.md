# DotaCard 网络传输协议 #
---

## 服务端 ##

*收到其他的Json，都原封不动传给对方玩家*

----------

- 收到玩家1的command 1000消息后，推送玩家2的卡组信息

        "command", 2000 "deck", deck2

- 收到玩家2的command 1000消息后，推送玩家1的卡组信息

        "command", 3000 "deck", deck2

- 玩家2已装载完玩家1的卡组，这时候应该通知玩家1，游戏开始

    	“command”，10000






## 客户端 ##

*以下都是客户端主动发送的消息，并不是客户端接受到的消息*

----------

- connected server后, 把本地从txt中读取的卡组信息发送给服务端

    	"command", 1000 "cards", cards

- 玩家2已装载完玩家1的卡组，让服务器通知玩家1开始游戏流程

    	“command”，1001

- 玩家1抽5张牌，告诉玩家2

    	"command", 10001
    	"cards", cards

- 玩家2抽5张牌，告诉玩家1

    	"command", 10002
    	"cards", cards

- 玩家1进入myDP流程

    	"command", 20001

- 玩家2进入yourDP流程

    	"command", 20002

- 玩家1进入mySP流程

    	"command", 30001

- 玩家2进入yourSP流程

    	"command", 30002

- 玩家1进入myM1流程

    	"command", 40001

- 玩家2进入yourM1流程

    	"command", 40002

- 玩家1进入myBP流程

    	"command", 50001

- 玩家2进入yourBP流程

    	"command", 50002

- 玩家1进入myM2流程

    	"command", 60001

- 玩家2进入yourM2流程

    	"command", 60002

- 玩家1进入myEP流程

    	"command", 70001

- 玩家2进入yourEP流程

    	"command", 70002

- 玩家在主要流程可以进行的操作
	
	- click on hand

		- normal summon

		 		"command", 41001
				"card" int
				"place" int (手牌中第几张 from 0)

		- special summon

		 		"command", 41002
				"card" int
				"place" int (手牌中第几张 from 0)

		- ~~tribute summon~~

		 		"command", 41003
				"card" int
				"place" int (手牌中第几张 from 0)

		- active monster

		 		"command", 41011
				"card" int
				"place" int (手牌中第几张 from 0)

		- active spell

		 		"command", 41012
				"card" int
				"place" int (手牌中第几张 from 0)

		- set monster

		 		"command", 41021
				"card" int
				"place" int (手牌中第几张 from 0)

		- set spell

		 		"command", 41022
				"card" int
				"place" int (手牌中第几张 from 0)

		- set trap

		 		"command", 41023
				"card" int
				"place" int (手牌中第几张 from 0)

	- click on field

		- flip summon

		 		"command", 42001
				"card" int
				"place" int (前场中第几张 from 0 to 4)

		- active monster

		 		"command", 42011
				"card" int
				"place" int (前场中第几张 from 0 to 4)

		- active spell

		 		"command", 42012
				"card" int
				"place" int (后场中第几张 from 5 to 9)

		- active trap

		 		"command", 42013
				"card" int
				"place" int (后场中第几张 from 5 to 9)

		- change from offensive to defensive

		 		"command", 42021
				"card" int
				"place" int (前场中第几张 from 0 to 4)

		- change from defensive to offensive

		 		"command", 42022
				"card" int
				"place" int (前场中第几张 from 0 to 4)

	- ~~click in grave~~

	- ~~click in deck~~

- 玩家在战斗流程可以进行的操作

	- 发动速攻魔法卡

			"command", 50001
			"card" int
			"place" int (后场中第几张 from 5 to 9)

	- 发动陷阱卡

	 		"command", 50002
			"card" int
			"place" int (后场中第几张 from 5 to 9)
