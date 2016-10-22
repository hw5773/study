from wand.image import Image
from wand.display import display

with Image(filename="et001.jpg") as img:
	print(img.size)

	for r in 1, 2, 3:
		with img.clone() as i:
			i.resize(int(i.width * r * 0.25), int(i.height * r * 0.25))
			i.rotate(90 * r)
			i.save(filename="et001-{0}.png".format(r))
			display(i)
