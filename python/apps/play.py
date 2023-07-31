from _arve import JungfrauFrameGrabber

g = JungfrauFrameGrabber("127.0.0.1", "50001")

f = g.recv()