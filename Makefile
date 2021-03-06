CONFIG_MODULE_SIG = n
TARGET_MODULE := fibdrv

obj-m := $(TARGET_MODULE).o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

MYCFLAGS := -std=c99 -Wall -Werror

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS)
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) client out transfer *.png apa test time
load:
	sudo insmod $(TARGET_MODULE).ko
unload:
	sudo rmmod $(TARGET_MODULE) || true >/dev/null

client: client.c
	$(CC) -o $@ $^

transfer: transfer.c
	$(CC) $(MYCFLAGS) -o $@ $^

plot: time.gp time
	@gnuplot $<
	@eog time.png

fibplot: fibtime.gp result.txt
	@gnuplot $<
	@eog fibtime.png

apa: apa.c apa.h
	$(CC) $(MYCFLAGS) -o $@ $^

test: apa.h test.c
	$(CC) $(MYCFLAGS) -o $@ $^

vtest: test.txt verificationTest.py
	python3 verificationTest.py

ktest: kresult.txt kernelverification.py
	python3 kernelverification.py

verify: result.txt verification.py
	python3 verification.py

test.txt: test.c apa.h
	$(CC) $(MYCFLAGS) -o test $^
	./test > /dev/null

result.txt: apa.h apa.c
	$(CC) $(MYCFLAGS) -o apa $^
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
