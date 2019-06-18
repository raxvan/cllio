
import srcbuild

ctx = srcbuild.Generator()

ctx.run("lib",
	incl = [
		"src"
	],
	src = ctx.glob("src",[".cpp",".h"])
)
