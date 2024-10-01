#include "plugin.hpp"


struct Harm_osc : Module {
	enum ParamId {
		VCA1_PARAM,
		VCA2_PARAM,
		VCA3_PARAM,
		VCA4_PARAM,
		VCA5_PARAM,
		VCA6_PARAM,
		VCA7_PARAM,
		VCA8_PARAM,
		FREQ_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		VOCT_INPUT,
		CV1_INPUT,
		CV2_INPUT,
		CV3_INPUT,
		CV4_INPUT,
		CV5_INPUT,
		CV6_INPUT,
		CV7_INPUT,
		CV8_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	float phase = 0.f;

	Harm_osc() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(VCA1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA2_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA3_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA4_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA5_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA6_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA7_PARAM, 0.f, 1.f, 0.f, "");
		configParam(VCA8_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configInput(VOCT_INPUT, "");
		configInput(CV1_INPUT, "");
		configInput(CV2_INPUT, "");
		configInput(CV3_INPUT, "");
		configInput(CV4_INPUT, "");
		configInput(CV5_INPUT, "");
		configInput(CV6_INPUT, "");
		configInput(CV7_INPUT, "");
		configInput(CV8_INPUT, "");
		configOutput(OUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		/*
		float baseFreq = params[FREQ_PARAM].getValue();
		float outputSignal = 0.0f;

		for (int i = 0; i < 8; i++) {
				float harmFreq = baseFreq * (i + 1);

				float amp = params[VCA1_PARAM + i].getValue();

				phase += harmFreq * args.sampleTime;
				if (phase >= 1.f) { phase -= 1.f; }

				float sine = std::sin(2.f * M_PI * phase);

				float harmSignal = amp * sine;
				outputSignal += harmSignal;
		}

		outputs[OUT_OUTPUT].setVoltage(outputSignal);*/
	}
};


struct Harm_oscWidget : ModuleWidget {
	Harm_oscWidget(Harm_osc* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/harm-osc.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(25.0, 30.0)), module, Harm_osc::VCA1_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(45.0, 30.0)), module, Harm_osc::VCA2_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(65.0, 30.0)), module, Harm_osc::VCA3_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(85.0, 30.0)), module, Harm_osc::VCA4_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(105.0, 30.0)), module, Harm_osc::VCA5_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(125.0, 30.0)), module, Harm_osc::VCA6_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(145.0, 30.0)), module, Harm_osc::VCA7_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(165.0, 30.0)), module, Harm_osc::VCA8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(14.0, 59.0)), module, Harm_osc::FREQ_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.0, 79.0)), module, Harm_osc::VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(29.0, 94.0)), module, Harm_osc::CV1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.0, 94.0)), module, Harm_osc::CV2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(69.0, 94.0)), module, Harm_osc::CV3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(89.0, 94.0)), module, Harm_osc::CV4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(109.0, 94.0)), module, Harm_osc::CV5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(129.0, 94.0)), module, Harm_osc::CV6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(149.0, 94.0)), module, Harm_osc::CV7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(169.0, 94.0)), module, Harm_osc::CV8_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(169.0, 114.0)), module, Harm_osc::OUT_OUTPUT));
	}
};


Model* modelHarm_osc = createModel<Harm_osc, Harm_oscWidget>("harm-osc");
