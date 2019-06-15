
import srcbuild

ctx = srcbuild.Generator()

ctx.run(
	kind = "lib",
	incl = [
		"src"
	],
	src = 
		ctx.glob("src",[".cpp",".h"])
)
