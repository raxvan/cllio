
def configure(cfg):
	cfg.link("../../ttf/testing.pak.py").enable()
	cfg.link("cllio.pak.py")
	cfg.type("exe")

def construct(ctx):
	ctx.fscan("src: ../tests/basic/")



