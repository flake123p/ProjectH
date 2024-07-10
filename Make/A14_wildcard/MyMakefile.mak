
# Wild Card
x = $(wildcard *.bat)
y = *.bat

all :
	@echo wildcard *.bat = $(x)
	@echo x = $(x)
	@echo y = $(y)