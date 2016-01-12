# DotaCardDuel
游戏王 for DotA with Qt GUI Framework

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
