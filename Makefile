CONFIG_MODULE_SIG = n
TARGET_MODULE := fibdrv

CFLAGS := -std=c99 -Wall -Werror

obj-m := $(TARGET_MODULE).o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) client transfer
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) client out transfer *.png apa test
load:
	sudo insmod $(TARGET_MODULE).ko
unload:
	sudo rmmod $(TARGET_MODULE) || true >/dev/null

client: client.c
	$(CC) -o $@ $^

transfer: transfer.c
	$(CC) $(CFLAGS) -o $@ $^

plot: time.gp time
	@gnuplot $<
	@eog time.png

apa: apa.c apa.h
	$(CC) $(CFLAGS) -o $@ $^

test: apa.h test.c
	$(CC) $(CFLAGS) -o $@ $^

vtest: test.txt verificationTest.py
	python3 verificationTest.py

verify: result.txt verification.py
	python3 verification.py

test.txt: test.c apa.h
	$(CC) $(CFLAGS) -o test $^
	./test > /dev/null

result.txt: apa.h apa.c
	$(CC) $(CFLAGS) -o apa $^
	./apa > /dev/null

PRINTF = env printf
PASS_COLOR = \e[32;01m
NO_COLOR = \e[0m
pass = $(PRINTF) "$(PASS_COLOR)$1 Passed [-]$(NO_COLOR)\n"

check: all
	$(MAKE) unload
	$(MAKE) load
	sudo ./client > out
	$(MAKE) unload
	@diff -u out scripts/expected.txt && $(call pass)
	@scripts/verify.py
