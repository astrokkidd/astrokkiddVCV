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

	#define NUM_WAVE_SAMPLES 1000

	float sineWaveTable[NUM_WAVE_SAMPLES];

	float harmonicLevels[8];
	float harmonicPhases[8] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};  // Up to 8 harmonics

	float mapToTable(float phase) {
		int tableIndex = (int)(phase * NUM_WAVE_SAMPLES);
		tableIndex = tableIndex % NUM_WAVE_SAMPLES;

		return (sineWaveTable[tableIndex]);
	}

	void initializeTable()
	{
		for (int i = 0; i < NUM_WAVE_SAMPLES; i++)
		{
			sineWaveTable[i] = 5.0f * std::sin(2.f * M_PI * ((float)i / NUM_WAVE_SAMPLES));
		}
	}

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
		configParam(FREQ_PARAM, -5.f, 5.f, 0.f, "Freq (1V/Oct)", " Hz", 2, dsp::FREQ_C4);
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

		initializeTable();
	}

	void process(const ProcessArgs& args) override {
		
		float baseFreq = params[FREQ_PARAM].getValue();
		float tempOut = 0.f;

		if (baseFreq < 10.f)
				baseFreq = 10.f;
			else if (baseFreq > 20000.f)
				baseFreq = 20000.f;

		harmonicPhases[0] += baseFreq * args.sampleTime;
			if (harmonicPhases[0] >= 1.f)
				harmonicPhases[0] -= 1.f;
		
		float outputSignal = 0.f;		

		for (int i = 0; i < 8; i++) {
			
			float harmonicFreq = baseFreq * (i + 1);

			if (getInput(CV1_INPUT + i).isConnected())
				harmonicLevels[i] = 0.1f * getInput(CV1_INPUT + i).getVoltage();
			else
				harmonicLevels[i] = getParam(VCA1_PARAM + i).getValue();

			tempOut += mapToTable((i+1) * harmonicPhases[0]);
		}

		// Output final signal
		outputs[OUT_OUTPUT].setVoltage(5.f * tempOut);  // Scale output to appropriate voltage range
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

		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(28.0, 73.0)), module, Harm_osc::VCA1_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(43.0, 73.0)), module, Harm_osc::VCA2_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(58.0, 73.0)), module, Harm_osc::VCA3_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(73.0, 73.0)), module, Harm_osc::VCA4_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(88.0, 73.0)), module, Harm_osc::VCA5_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(103.0, 73.0)), module, Harm_osc::VCA6_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(118.0, 73.0)), module, Harm_osc::VCA7_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(133.0, 73.0)), module, Harm_osc::VCA8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(13.0, 73.0)), module, Harm_osc::FREQ_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.0, 88.0)), module, Harm_osc::VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.0, 103.0)), module, Harm_osc::CV1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.0, 103.0)), module, Harm_osc::CV2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(58.0, 103.0)), module, Harm_osc::CV3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(73.0, 103.0)), module, Harm_osc::CV4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(88.0, 103.0)), module, Harm_osc::CV5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(103.0, 103.0)), module, Harm_osc::CV6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(118.0, 103.0)), module, Harm_osc::CV7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(133.0, 103.0)), module, Harm_osc::CV8_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(133.0, 118.0)), module, Harm_osc::OUT_OUTPUT));

		/*		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(13.0, 73.0)), module, Harmosc::FREQ_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(28.0, 73.0)), module, Harmosc::VCA1_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(43.0, 73.0)), module, Harmosc::VCA2_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(58.0, 73.0)), module, Harmosc::VCA3_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(73.0, 73.0)), module, Harmosc::VCA4_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(88.0, 73.0)), module, Harmosc::VCA5_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(103.0, 73.0)), module, Harmosc::VCA6_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(118.0, 73.0)), module, Harmosc::VCA7_PARAM));
		addParam(createParamCentered<BefacoSlidePot>(mm2px(Vec(133.0, 73.0)), module, Harmosc::VCA8_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.0, 88.0)), module, Harmosc::VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.0, 103.0)), module, Harmosc::CV1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.0, 103.0)), module, Harmosc::CV2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(58.0, 103.0)), module, Harmosc::CV3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(73.0, 103.0)), module, Harmosc::CV4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(88.0, 103.0)), module, Harmosc::CV5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(103.0, 103.0)), module, Harmosc::CV6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(118.0, 103.0)), module, Harmosc::CV7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(133.0, 103.0)), module, Harmosc::CV8_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(133.0, 118.0)), module, Harmosc::OUT_OUTPUT));*/

	}
};


Model* modelHarm_osc = createModel<Harm_osc, Harm_oscWidget>("harm-osc");
