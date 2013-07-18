# S.Chekanov. ANL
# K.Strand. WSU

ifndef PROMC
$(error PROMC env variable is not set. Run setup.sh first)
endif

all:
	make -f Makefile_pyt
	make -f Makefile_event

clean:
	make -f Makefile_pyt clean;
	make -f Makefile_event clean;
