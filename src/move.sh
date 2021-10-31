i=0
dest=""

for arg in "$@"
do
	if [[ $i -eq 0  ]]
	then
		dest=$arg
	else
		mv $arg "$dest/$arg"
	fi
	
	i+=1
done
