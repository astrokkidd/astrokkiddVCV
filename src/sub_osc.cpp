#include "plugin.hpp"
#include <cmath>


struct Sub_osc : Module {
	enum ParamId {
		OSC1_FREQ_PARAM,
		OSC2_FREQ_PARAM,
		OSC3_FREQ_PARAM,
		OSC1_TRI_LEVEL_PARAM,
		OSC2_TRI_LEVEL_PARAM,
		OSC3_TRI_LEVEL_PARAM,
		OSC1_WIDTH_PARAM,
		OSC2_WIDTH_PARAM,
		OSC3_WIDTH_PARAM,
		OSC1_SAW_LEVEL_PARAM,
		OSC2_SAW_LEVEL_PARAM,
		OSC3_SAW_LEVEL_PARAM,
		OSC1_PWM_PARAM,
		OSC2_PWM_PARAM,
		OSC3_PWM_PARAM,
		OSC1_SQR_LEVEL_PARAM,
		OSC2_SQR_LEVEL_PARAM,
		OSC3_SQR_LEVEL_PARAM,
		ENV_ATTACK_PARAM,
		ENV_DECAY_PARAM,
		LFO2_FREQ_PARAM,
		LFO1_LEVEL_PARAM,
		FILTER_CUTOFF_PARAM,
		ENV_SUSTAIN_PARAM,
		LFO1_FREQ_PARAM,
		LFO2_LEVEL_PARAM,
		ENV_RELEASE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		OSC1_VOCT_INPUT,
		OSC2_VOCT_INPUT,
		OSC3_VOCT_INPUT,
		OSC1_PWM_CV_INPUT,
		OSC2_PWM_CV_INPUT,
		OSC3_PWM_CV_INPUT,
		LFO1_VOCT_INPUT,
		FILTER_CUT_CV_INPUT,
		LFO2_VOCT_INPUT,
		AMP_CV_INPUT,
		FILTER_IN_INPUT,
		FILTER_LP_INPUT,
		AMP_IN_INPUT,
		ENV_GATE_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OSC1_TRI_OUTPUT,
		OSC2_TRI_OUTPUT,
		OSC3_TRI_OUTPUT,
		OSC1_SAW_OUTPUT,
		OSC2_SAW_OUTPUT,
		OSC3_SAW_OUTPUT,
		OSC1_SQR_OUTPUT,
		OSC2_SQR_OUTPUT,
		OSC3_SQR_OUTPUT,
		LFO2_OUT_OUTPUT,
		LFO1_OUT_OUTPUT,
		AMP_OUT_OUTPUT,
		ENV_OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Sub_osc() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(OSC1_FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC2_FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC3_FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC1_TRI_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC2_TRI_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC3_TRI_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC1_WIDTH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC2_WIDTH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC3_WIDTH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC1_SAW_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC2_SAW_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC3_SAW_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC1_PWM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC2_PWM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC3_PWM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC1_SQR_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC2_SQR_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OSC3_SQR_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ENV_ATTACK_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ENV_DECAY_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LFO2_FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LFO1_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FILTER_CUTOFF_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ENV_SUSTAIN_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LFO1_FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LFO2_LEVEL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ENV_RELEASE_PARAM, 0.f, 1.f, 0.f, "");
		configInput(OSC1_VOCT_INPUT, "");
		configInput(OSC2_VOCT_INPUT, "");
		configInput(OSC3_VOCT_INPUT, "");
		configInput(OSC1_PWM_CV_INPUT, "");
		configInput(OSC2_PWM_CV_INPUT, "");
		configInput(OSC3_PWM_CV_INPUT, "");
		configInput(LFO1_VOCT_INPUT, "");
		configInput(FILTER_CUT_CV_INPUT, "");
		configInput(LFO2_VOCT_INPUT, "");
		configInput(AMP_CV_INPUT, "");
		configInput(FILTER_IN_INPUT, "");
		configInput(FILTER_LP_INPUT, "");
		configInput(AMP_IN_INPUT, "");
		configInput(ENV_GATE_INPUT, "");
		configOutput(OSC1_TRI_OUTPUT, "");
		configOutput(OSC2_TRI_OUTPUT, "");
		configOutput(OSC3_TRI_OUTPUT, "");
		configOutput(OSC1_SAW_OUTPUT, "");
		configOutput(OSC2_SAW_OUTPUT, "");
		configOutput(OSC3_SAW_OUTPUT, "");
		configOutput(OSC1_SQR_OUTPUT, "");
		configOutput(OSC2_SQR_OUTPUT, "");
		configOutput(OSC3_SQR_OUTPUT, "");
		configOutput(LFO2_OUT_OUTPUT, "");
		configOutput(LFO1_OUT_OUTPUT, "");
		configOutput(AMP_OUT_OUTPUT, "");
		configOutput(ENV_OUT_OUTPUT, "");
	}

	float phase1 = 0.f, phase2 = 0.f, phase3 = 0.f;
    float lfo1_phase = 0.f, lfo2_phase = 0.f;
    
    // State variables for envelope
    float env_out = 0.f;
    bool gate_state = false;
    enum EnvStage { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };
    EnvStage env_stage = IDLE;
    
    // Filter state
    float filter_state = 0.f;
    
    // Helper functions
    float sawWave(float phase) {
        return 2.f * phase - 1.f;
    }
    
    float triWave(float phase) {
        return 4.f * std::abs(phase - 0.5f) - 1.f;
    }
    
    float sqrWave(float phase, float width) {
        return phase < width ? 1.f : -1.f;
    }
    
    void process(const ProcessArgs& args) override {
        // Get sample time
        float dt = args.sampleTime;
        
        // Process Oscillators
        for (int osc = 0; osc < 3; osc++) {
            // Calculate base frequency for each oscillator
            float freq_param = params[OSC1_FREQ_PARAM + osc].getValue();
            float voct = inputs[OSC1_VOCT_INPUT + osc].getVoltage();
            float freq = 440.f * std::pow(2.f, voct + freq_param * 4.f - 4.f);
            
            // Get phase pointers
            float* phase = (osc == 0) ? &phase1 : (osc == 1) ? &phase2 : &phase3;
            
            // Increment phase
            *phase += freq * dt;
            if (*phase >= 1.f)
                *phase -= 1.f;
            
            // Get PWM values
            float width = params[OSC1_WIDTH_PARAM + osc].getValue();
            if (inputs[OSC1_PWM_CV_INPUT + osc].isConnected()) {
                width += inputs[OSC1_PWM_CV_INPUT + osc].getVoltage() / 10.f;
            }
            width = clamp(width, 0.1f, 0.9f);
            
            // Generate waveforms
            float tri = triWave(*phase) * params[OSC1_TRI_LEVEL_PARAM + osc].getValue();
            float saw = sawWave(*phase) * params[OSC1_SAW_LEVEL_PARAM + osc].getValue();
            float sqr = sqrWave(*phase, width) * params[OSC1_SQR_LEVEL_PARAM + osc].getValue();
            
            // Output waveforms
            outputs[OSC1_TRI_OUTPUT + osc].setVoltage(5.f * tri);
            outputs[OSC1_SAW_OUTPUT + osc].setVoltage(5.f * saw);
            outputs[OSC1_SQR_OUTPUT + osc].setVoltage(5.f * sqr);
        }
        
        // Process LFOs
        for (int lfo = 0; lfo < 2; lfo++) {
            float* phase = (lfo == 0) ? &lfo1_phase : &lfo2_phase;
            float freq = params[LFO1_FREQ_PARAM + lfo].getValue();
            if (inputs[LFO2_VOCT_INPUT + lfo].isConnected()) {
                freq *= std::pow(2.f, inputs[LFO2_VOCT_INPUT + lfo].getVoltage());
            }
            
            *phase += freq * dt;
            if (*phase >= 1.f)
                *phase -= 1.f;
            
            float tri = triWave(*phase) * params[LFO1_LEVEL_PARAM + lfo].getValue();
            outputs[LFO1_OUT_OUTPUT + lfo].setVoltage(5.f * tri);
        }
        
        // Process Envelope
        bool gate = inputs[ENV_GATE_INPUT].getVoltage() >= 1.f;
        float attack = params[ENV_ATTACK_PARAM].getValue() * 10.f;
        float decay = params[ENV_DECAY_PARAM].getValue() * 10.f;
        float sustain = params[ENV_SUSTAIN_PARAM].getValue();
        float release = params[ENV_RELEASE_PARAM].getValue() * 10.f;
        
        // Gate transition
        if (gate && !gate_state) {
            env_stage = ATTACK;
        }
        else if (!gate && gate_state) {
            env_stage = RELEASE;
        }
        gate_state = gate;
        
        // Envelope stage processing
        switch (env_stage) {
            case ATTACK:
                env_out += dt / attack;
                if (env_out >= 1.f) {
                    env_out = 1.f;
                    env_stage = DECAY;
                }
                break;
                
            case DECAY:
                env_out -= dt / decay * (1.f - sustain);
                if (env_out <= sustain) {
                    env_out = sustain;
                    env_stage = SUSTAIN;
                }
                break;
                
            case SUSTAIN:
                env_out = sustain;
                break;
                
            case RELEASE:
                env_out -= dt / release * env_out;
                if (env_out <= 0.001f) {
                    env_out = 0.f;
                    env_stage = IDLE;
                }
                break;
                
            default: // IDLE
                env_out = 0.f;
                break;
        }
        
        outputs[ENV_OUT_OUTPUT].setVoltage(10.f * env_out);
        
        // Process Filter
        if (inputs[FILTER_IN_INPUT].isConnected()) {
            float cutoff = params[FILTER_CUTOFF_PARAM].getValue();
            if (inputs[FILTER_CUT_CV_INPUT].isConnected()) {
                cutoff = clamp(cutoff + inputs[FILTER_CUT_CV_INPUT].getVoltage() / 10.f, 0.f, 1.f);
            }
            
            // Simple one-pole lowpass filter
            float alpha = dt * 20000.f * std::pow(cutoff, 2.f);
            filter_state += alpha * (inputs[FILTER_IN_INPUT].getVoltage() - filter_state);
            outputs[FILTER_LP_INPUT].setVoltage(filter_state);
        }
        
        // Process Amplifier
        if (inputs[AMP_IN_INPUT].isConnected()) {
            float cv = 1.f;
            if (inputs[AMP_CV_INPUT].isConnected()) {
                cv *= clamp(inputs[AMP_CV_INPUT].getVoltage() / 10.f, 0.f, 1.f);
            }
            outputs[AMP_OUT_OUTPUT].setVoltage(inputs[AMP_IN_INPUT].getVoltage() * cv);
        }
    }
};


struct Sub_oscWidget : ModuleWidget {
	Sub_oscWidget(Sub_osc* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/sub_osc.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 10.0)), module, Sub_osc::OSC1_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 10.0)), module, Sub_osc::OSC2_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(127.0, 10.0)), module, Sub_osc::OSC3_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 18.0)), module, Sub_osc::OSC1_TRI_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 18.0)), module, Sub_osc::OSC2_TRI_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(153.0, 18.0)), module, Sub_osc::OSC3_TRI_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 25.0)), module, Sub_osc::OSC1_WIDTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 25.0)), module, Sub_osc::OSC2_WIDTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(127.0, 25.0)), module, Sub_osc::OSC3_WIDTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 33.0)), module, Sub_osc::OSC1_SAW_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 33.0)), module, Sub_osc::OSC2_SAW_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(153.0, 33.0)), module, Sub_osc::OSC3_SAW_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 40.0)), module, Sub_osc::OSC1_PWM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 40.0)), module, Sub_osc::OSC2_PWM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(127.0, 40.0)), module, Sub_osc::OSC3_PWM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 48.0)), module, Sub_osc::OSC1_SQR_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 48.0)), module, Sub_osc::OSC2_SQR_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(153.0, 48.0)), module, Sub_osc::OSC3_SQR_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 60.0)), module, Sub_osc::ENV_ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 75.0)), module, Sub_osc::ENV_DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(62.0, 75.0)), module, Sub_osc::LFO2_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(88.0, 75.0)), module, Sub_osc::LFO1_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(114.0, 75.0)), module, Sub_osc::FILTER_CUTOFF_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 90.0)), module, Sub_osc::ENV_SUSTAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(62.0, 90.0)), module, Sub_osc::LFO1_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(88.0, 90.0)), module, Sub_osc::LFO2_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 105.0)), module, Sub_osc::ENV_RELEASE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.0, 10.0)), module, Sub_osc::OSC1_VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.0, 10.0)), module, Sub_osc::OSC2_VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.0, 10.0)), module, Sub_osc::OSC3_VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.0, 40.0)), module, Sub_osc::OSC1_PWM_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.0, 40.0)), module, Sub_osc::OSC2_PWM_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.0, 40.0)), module, Sub_osc::OSC3_PWM_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.0, 75.0)), module, Sub_osc::LFO1_VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(101.0, 75.0)), module, Sub_osc::FILTER_CUT_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.0, 90.0)), module, Sub_osc::LFO2_VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(127.0, 90.0)), module, Sub_osc::AMP_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(101.0, 105.0)), module, Sub_osc::FILTER_IN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.0, 105.0)), module, Sub_osc::FILTER_LP_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(127.0, 105.0)), module, Sub_osc::AMP_IN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.0, 115.0)), module, Sub_osc::ENV_GATE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 18.0)), module, Sub_osc::OSC1_TRI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.0, 18.0)), module, Sub_osc::OSC2_TRI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 18.0)), module, Sub_osc::OSC3_TRI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 33.0)), module, Sub_osc::OSC1_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.0, 33.0)), module, Sub_osc::OSC2_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 33.0)), module, Sub_osc::OSC3_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 48.0)), module, Sub_osc::OSC1_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.0, 48.0)), module, Sub_osc::OSC2_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 48.0)), module, Sub_osc::OSC3_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 75.0)), module, Sub_osc::LFO2_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 90.0)), module, Sub_osc::LFO1_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 105.0)), module, Sub_osc::AMP_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 115.0)), module, Sub_osc::ENV_OUT_OUTPUT));
	}
};


Model* modelSub_osc = createModel<Sub_osc, Sub_oscWidget>("sub_osc");