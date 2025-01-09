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
		STATE_PARAM,
		REG_PARAM,
		LOG_PARAM,
		POW_PARAM,
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
	enum LightIds {
		REG_LIGHT,
		LOG_LIGHT,
		SQT_LIGHT,
		LIGHTS_LEN
	};

	float phase = 0.f;

	#define NUM_WAVE_SAMPLES 1000

	float sineWaveTable[NUM_WAVE_SAMPLES];

	float harmonicLevels[8];
	float harmonicPhases[8] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

	float stdHarmonicFrequencies[8] = {261.63, 523.28, 784.89, 1046.73, 1308.15, 1569.78, 1831.41, 2093.04 };
	float logHarmonicFrequencies[8] = {261.63, 523.28, 905.24, 1046.73, 1469.38,  1810.5, 1993.92, 2094.88 };
	float sqtHarmonicFrequencies[8] = {261.63, 523.28, 867.82, 1046.73, 1384.74, 1736.09, 1958.28, 2094.88 };

	bool isReg = true;
	bool isLog, isSqt = false;

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
		configParam(VCA8_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQ_PARAM, -5.f, 5.f, 0.f, "Freq (1V/Oct)", " Hz", 2, dsp::FREQ_C4);
		configSwitch(STATE_PARAM, 1.f, 3.f, 1.f, "Harmonic State");

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
    // Calculate the base frequency from parameters and inputs
		float baseFreq = dsp::FREQ_C4 * std::pow(2.f, params[FREQ_PARAM].getValue() + inputs[VOCT_INPUT].getVoltage());

		// Ensure base frequency is within a reasonable range
		baseFreq = clamp(baseFreq, 10.f, 20000.f);

		float outputSignal = 0.f;

		// Iterate over all harmonics
		for (int i = 0; i < 8; i++) {
			// Calculate harmonic frequency as (i + 1) times the base frequency
			float harmonicScale = 0.f;
			float harmonicState = params[STATE_PARAM].getValue();

			float harmonicFreq = 0.f;

			if (harmonicState == 1.f) {
				//harmonicScale = i + 1;
				harmonicFreq = stdHarmonicFrequencies[i];
			} else if (harmonicState == 2.f) {
				//harmonicScale = std::log2(i + 2);
				harmonicFreq = logHarmonicFrequencies[i];
			} else if (harmonicState == 3.f) {
				//harmonicScale = std::sqrt(i + 1);
				harmonicFreq = sqtHarmonicFrequencies[i];
			}

        	//float harmonicFreq = baseFreq * harmonicScale;

			// Adjust harmonic phase based on the current sample time
			harmonicPhases[i] += harmonicFreq * args.sampleTime;
			if (harmonicPhases[i] >= 1.f) {
				harmonicPhases[i] -= 1.f;
			}

			// Determine the amplitude of the harmonic from CV input or parameter
			float amplitude = inputs[CV1_INPUT + i].isConnected()
				? clamp(inputs[CV1_INPUT + i].getVoltage() * 0.1f, 0.f, 1.f)
				: params[VCA1_PARAM + i].getValue();

			// Generate the harmonic signal using the phase and amplitude
			float harmonicSignal = amplitude * mapToTable(harmonicPhases[i]);

			// Sum the harmonic signal into the total output signal
			outputSignal += harmonicSignal;
		}

		// Set the final output voltage, scaled to a reasonable range
		outputs[OUT_OUTPUT].setVoltage(5.f * outputSignal / 8.f);  // Normalize by number of harmonics
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

		addParam(createParamCentered<VCVSlider>(mm2px(Vec(28.0, 73.0)), module, Harm_osc::VCA1_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(43.0, 73.0)), module, Harm_osc::VCA2_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(58.0, 73.0)), module, Harm_osc::VCA3_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(73.0, 73.0)), module, Harm_osc::VCA4_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(88.0, 73.0)), module, Harm_osc::VCA5_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(103.0, 73.0)), module, Harm_osc::VCA6_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(118.0, 73.0)), module, Harm_osc::VCA7_PARAM));
		addParam(createParamCentered<VCVSlider>(mm2px(Vec(133.0, 73.0)), module, Harm_osc::VCA8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(13.0, 73.0)), module, Harm_osc::FREQ_PARAM));

		addParam(createParamCentered<CKSSThree>(mm2px(Vec(13.0, 58.0)), module, Harm_osc::STATE_PARAM));
		
		//addParam(createLightParamCentered<VCVLightLatch<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(13.0, 58.0)), module, Harm_osc::REG_PARAM, Harm_osc::REG_LIGHT));
		//addParam(createLightParamCentered<VCVLightLatch<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(13.0, 43.0)), module, Harm_osc::LOG_PARAM, Harm_osc::LOG_LIGHT));
		//addParam(createLightParamCentered<VCVLightLatch<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(13.0, 28.0)), module, Harm_osc::POW_PARAM, Harm_osc::POW_LIGHT));

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
