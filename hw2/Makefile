include ../common.mk

.PHONY: all clean

%: %.c
	@echo $<
	@echo $@
	$(CC) $< $(CFLAGS) -o $@

all: $(TARGETS)
	@echo $(TARGETS)

run: $(TARGETS)
	@for target in $(TARGETS); do \
		echo "Running $$target:"; \
		./$$target; \
	done

clean:
	rm -f $(TARGETS)