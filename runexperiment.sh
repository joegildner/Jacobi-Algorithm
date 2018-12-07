rm ./results.txt

for i in {1..32};
do
	echo "Running Jacobi's Algorithm with $i threads" >> results.txt
	echo "Running Jacobi's Algorithm with $i threads"
	for j in {1..3};
	do
		echo Running Trial $j >> results.txt
		echo Running Trial $j
		{ time ./jacobi $i ; } 2>> results.txt
	done
done
