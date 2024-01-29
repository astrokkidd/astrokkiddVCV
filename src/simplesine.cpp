#include "plugin.hpp"


struct SimpleSine : Module {
	enum ParamId {
		PITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		_1VOCT_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SINE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT_LIGHT,
		LIGHTS_LEN
	};

	SimpleSine() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
		configInput(_1VOCT_INPUT, "");
		configOutput(SINE_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct SimpleSineWidget : ModuleWidget {
	SimpleSineWidget(SimpleSine* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/SimpleSine.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 59.08)), module, SimpleSine::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.339, 81.982)), module, SimpleSine::_1VOCT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.339, 101.707)), module, SimpleSine::SINE_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 40.593)), module, SimpleSine::LIGHT_LIGHT));
	}
};


Model* modelSimpleSine = createModel<SimpleSine, SimpleSineWidget>("SimpleSine");