#include "plugin.hpp"


struct SimpleSine : Module {
	enum ParamId {
		PITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		PITCH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SINE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		BLINK_LIGHT,
		LIGHTS_LEN
	};

	float phase = 0.f;
	float blinkPhase = 0.f;

	SimpleSine() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PITCH_PARAM, -5.f, 5.f, 0.f, "Pitch (1V/Oct)", " Hz", 2, dsp::FREQ_C4);
		configInput(PITCH_INPUT, "1V/Octave pitch");
		configOutput(SINE_OUTPUT, "Sine");
	}

	void process(const ProcessArgs& args) override {

		float pitch = params[PITCH_PARAM].getValue();
		pitch += inputs[PITCH_INPUT].getVoltage();

		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

		phase += freq * args.sampleTime;
		if (phase >= 1.f) { phase -= 1.f; }

		float sine = std::sin(2.f * M_PI * phase);
		outputs[SINE_OUTPUT].setVoltage(5.f * sine);

		blinkPhase += args.sampleTime;
		if (blinkPhase >= 1.f) { blinkPhase -= 1.f; }
		lights[BLINK_LIGHT].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);

	}
};


struct SimpleSineWidget : ModuleWidget {
	SimpleSineWidget(SimpleSine* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/simplesine.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 59.08)), module, SimpleSine::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.339, 81.982)), module, SimpleSine::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.339, 101.707)), module, SimpleSine::SINE_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 40.593)), module, SimpleSine::BLINK_LIGHT));
	}
};


Model* modelSimpleSine = createModel<SimpleSine, SimpleSineWidget>("SimpleSine");