

def configure(cfg):
	cfg.link("public: ../../ttf/testing.pak.py").enable()
	cfg.link("cllio.pak.py")

def construct(ctx):

	ctx.config("type","exe")
	ctx.fscan("src: ../tests/basic/")

	
