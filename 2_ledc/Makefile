objs = start.o main.o

ledc.bin: $(objs)
	arm-linux-gnueabihf-ld -Timx6u.lds $^ -o ledc.elf
	arm-linux-gnueabihf-objcopy -O binary -S ledc.elf $@
	arm-linux-gnueabihf-objdump -D -m arm ledc.elf > ledc.dis

%.o: %.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c $< -o $@
%.o: %.S
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c $< -o $@

clean:
	rm -rf *.o ledc.elf ledc.bin ledc.dis

