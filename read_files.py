import pdb
import digital_rf as drf

do = drf.DigitalRFReader("/data/beacon/data")
s,e = do.get_bounds("ch0")

z = do.read_vector(s, 10, "ch0")

pdb.set_trace()
