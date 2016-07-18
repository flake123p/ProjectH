
--
[TCPClient]:
[TCPServer]:
TCP的基本範例
1.client 端每次送完一次訊息後就會 close socket
2.server 端每次收完一次訊息後也會 close socket, 但會再loop回去accept狀態
3.close()的使用有點自由，寫嚴謹程式時，需留意與更深入學習


--
[UDPPeer]:
UDP P2P的範例


--
補充資料:
Wiki - Berkeley Sockets
http://en.wikipedia.org/wiki/Berkeley_sockets



