
import srcbuild

ctx = srcbuild.Generator()

ctx.run("exe",
	diagnostics = True,
	src = ctx.glob("../tests/src",[".cpp",".h"]),
	depends = [
		"prj.cllio.py"
	]
)
