
import srcbuild

ctx = srcbuild.Generator()

ctx.puke(
	kind = "exe",
	incl = [
		"src"
	],
	src = 
		ctx.glob("src",[".cpp",".h"]) +
		ctx.glob("test/src",[".cpp",".h"])
	,
)
