import("stdfaust.lib");

frec_in = hgroup("MIDI", nentry("freq", 220, 20, 5000, 0.001));
gain_in = hgroup("MIDI", nentry("gain", 0.0, 0.0, 1.0, 0.001));
gate_in = hgroup("MIDI", nentry("gate", 0.0, 0.0, 1.0, 1.0));

envolvente = en.adsr(0.05, 0.01, 0.8, 0.025, gate_in);
sierra(f) = os.sawtooth(f)*gain_in*envolvente;

sinte = sierra(frec_in), sierra(frec_in*1.005);
process = sinte;