
import srcbuild

ctx = srcbuild.Generator()

ctx.run("exe",
	defines = [
		"global>TESTING_PROJECT",
	],
	src = ctx.glob("tests/src",[".cpp",".h"]),
	depends = [
		"prj.cllio.py"
	]
)
