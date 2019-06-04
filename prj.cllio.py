
import srcbuild

ctx = srcbuild.Generator()

ctx.puke(
	kind = "lib",
	incl = [
		"src"
	],
	src = 
		ctx.glob("src",[".cpp",".h"])
)
