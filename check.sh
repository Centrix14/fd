export FLAGS="--bug-hunting --enable=all --force \
	--language=c --std=c11"
export INPUT=src/

cppcheck $FLAGS $INPUT 2> cppcheck/results
