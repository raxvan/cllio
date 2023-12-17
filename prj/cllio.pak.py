

def configure(cfg):
	cfg.link_optional("../../ttf/testing.pak.py")
	cfg.link_optional("../../dev-platform/prj/dev-platform.pak.py")

def construct(ctx):
	
	ctx.assign_config_option("type","lib")

	ctx.folder("public include: ../incl")

	ctx.fscan("src: ../incl")
	ctx.fscan("src: ../src")

	if ctx.module_enabled("testing"):
		ctx.assign("public define:CLLIO_TESTING")

	if ctx.module_enabled("dev-platform"):
		ctx.assign("public define:CLLIO_WITH_DEV_PLATFORM")

