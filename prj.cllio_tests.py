
import srcbuild

ctx = srcbuild.Generator()

ctx.run("exe",
	src = ctx.glob("tests/src",[".cpp",".h"]),
	depends = [
		"prj.cllio.py"
	]
)
