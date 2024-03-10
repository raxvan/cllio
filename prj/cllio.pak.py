

def configure(cfg):
	cfg.trylink("../../ttf/testing.pak.py")
	cfg.trylink("../../dev-platform/prj/dev-platform.pak.py")
	cfg.type("lib")

def construct(ctx):
	
	ctx.folder("public include: ../incl")

	ctx.fscan("src: ../incl")
	ctx.fscan("src: ../src")

	if ctx.module_enabled("testing"):
		ctx.set("public define:CLLIO_TESTING")

	if ctx.module_enabled("dev-platform"):
		ctx.set("public define:CLLIO_WITH_DEV_PLATFORM")

