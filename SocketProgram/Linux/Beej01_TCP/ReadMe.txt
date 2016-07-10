
--
[TCPClientO]:
[TCPServerO]:
從Sample2拿去改成Beej上所說的新式風格，也就是使用 getaddrinfo()。
1.socket()參數有點變化，可參照被mark的舊code。
2.bind()參數也有變化，可參照被mark的舊code。
3.demo gethostname()函式的使用
4.demo getpeername()函式的使用


--
[1_showip]:
Beej上的第五章範例，demo getaddrinfo()的使用方式，可以把URL轉成IPv4與IPv6。
1.此程式需要帶引數，所以不能直接執行run.sh了(但還是可以用來build code)。
2.程式的要帶一個URL做為參數，範例:
./a.out www.google.com.tw


