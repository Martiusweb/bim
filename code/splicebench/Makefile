all:
	clang++ splice.cpp -o splice
	clang++ read_write.cpp -o read_write
	./generate_dummy_files.sh
	./bench.sh 0 50 ./read_write ./splice

clean:
	rm file1
	rm file2
	rm file1rw
	rm file2rw
	rm read_write
	rm splice
	rm benchtool/*.pyc
	rm *.svg
	rm *.dat
