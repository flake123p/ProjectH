
--
[ChatClient]:
[ChatServer]:
改良Sample 3，用 select() 實作聊天室，

1.Server可以多工地收發message，因為使用select()而不需開multi-thread!!
2.Client則無法不開multi-thread而多工收發message，因為會卡在scanf()
  (multi-thread範例在後面)
3.故這邊有兩個Client程式，Client1負責"送"message，
  Client2這個程式只負責"收"message。
4.Demo流程:
	先開 Server
	再開兩個Client2
	再開一個Client1送message
	接下來可以在兩個Client2上觀察到，都收到Message
	基本上可以把Client2當作聊天室的顯示視窗，Client1就是send字串的代理人

p.s.
Client1 就是 ChatClient 資料夾
Client2 就是 ChatClinet2 資料夾
