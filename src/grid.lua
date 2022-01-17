local width, height, step

width = 1200
height = 900
step = 80

local x, y
x = 0
y = 0

local i, j = 0, 0
while y < height
do
	y = step * i
	x = 0
	j = 0

	while x < width
	do
		x = step * j
		j = j + 1

		point(x, y)
	end
	
	i = i + 1
end
