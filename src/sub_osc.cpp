#include "plugin.hpp"
#include <cmath>

// Simple oscillator class
struct BasicOscillator {
    float phase = 0.f;
    float freq = 0.f;
    float pw = 0.5f;
    
    void updatePhase(float deltaTime) {
        phase += freq * deltaTime;
        if (phase >= 1.f)
            phase -= 1.f;
    }
    
    float sine() {
        return std::sin(2.f * M_PI * phase);
    }
    
    float triangle() {
        float t = phase;
        if (t < 0.5f)
            return 4.f * t - 1.f;
        else
            return 3.f - 4.f * t;
    }
    
    float saw() {
        return 2.f * phase - 1.f;
    }
    
    float square() {
        return (phase < pw) ? 1.f : -1.f;
    }
};

// Simple ADSR envelope
struct ADSREnvelope {
    enum Stage {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };
    
    Stage stage = IDLE;
    float output = 0.f;
    float attackTime = 0.1f;
    float decayTime = 0.3f;
    float sustainLevel = 0.5f;
    float releaseTime = 1.0f;
    float stageProgress = 0.f;
    bool gateWasHigh = false;
    
    void process(float deltaTime, bool gate) {
        // State transitions
        if (gate && !gateWasHigh) {
            // Gate rising edge
            stage = ATTACK;
            stageProgress = 0.f;
        }
        else if (!gate && gateWasHigh) {
            // Gate falling edge
            if (stage != IDLE) {
                stage = RELEASE;
                stageProgress = 0.f;
            }
        }
        
        gateWasHigh = gate;
        
        // State processing
        switch (stage) {
            case IDLE:
                output = 0.f;
                break;
                
            case ATTACK:
                stageProgress += deltaTime;
                output = stageProgress / attackTime;
                if (output >= 1.f) {
                    output = 1.f;
                    stage = DECAY;
                    stageProgress = 0.f;
                }
                break;
                
            case DECAY:
                stageProgress += deltaTime;
                output = 1.f - (1.f - sustainLevel) * (stageProgress / decayTime);
                if (stageProgress >= decayTime) {
                    output = sustainLevel;
                    stage = SUSTAIN;
                }
                break;
                
            case SUSTAIN:
                output = sustainLevel;
                break;
                
            case RELEASE:
                stageProgress += deltaTime;
                output = sustainLevel * (1.f - stageProgress / releaseTime);
                if (stageProgress >= releaseTime) {
                    output = 0.f;
                    stage = IDLE;
                }
                break;
        }
    }
};

// One-pole low-pass filter
struct OnePoleFilter {
    float output = 0.f;
    float cutoff = 1000.f;
    float sampleRate = 44100.f;
    
    void process(float input) {
        // Convert cutoff frequency to coefficient
        float rc = 1.f / (2.f * M_PI * cutoff);
        float dt = 1.f / sampleRate;
        float alpha = dt / (rc + dt);
        
        // Apply filter
        output = output + alpha * (input - output);
    }
};

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
        AMP_LEVEL_PARAM,
        ENV_RELEASE_PARAM,
        LFO1_FREQ_PARAM,
        LFO2_LEVEL_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        VOCT_INPUT,
        OSC1_PWM_CV_INPUT,
        OSC2_PWM_CV_INPUT,
        OSC3_PWM_CV_INPUT,
        FILTER_CUT_CV_INPUT,
        ENV_GATE_INPUT,
        AMP_CV_INPUT,
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
        ENV_OUT_OUTPUT,
        LFO1_OUT_OUTPUT,
        AMP_OUT_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    // Oscillators
    BasicOscillator osc1;
    BasicOscillator osc2;
    BasicOscillator osc3;
    BasicOscillator lfo1;
    BasicOscillator lfo2;
    
    // Envelope
    ADSREnvelope env;
    
    // Filter
    OnePoleFilter filter;
    
    Sub_osc() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configParam(OSC1_FREQ_PARAM, -5.f, 5.f, 0.f, "Osc 1 Pitch (1V/Oct)", " Hz", 2, dsp::FREQ_C4);
        configParam(OSC2_FREQ_PARAM, -5.f, 5.f, 0.f, "Osc 2 Pitch (1V/Oct)", " Hz", 2, dsp::FREQ_C4);
        configParam(OSC3_FREQ_PARAM, -5.f, 5.f, 0.f, "Osc 3 Pitch (1V/Oct)", " Hz", 2, dsp::FREQ_C4);
        configParam(OSC1_TRI_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 1 Triangle Level", "%", 0.f, 100.f);
        configParam(OSC2_TRI_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 2 Triangle Level", "%", 0.f, 100.f);
        configParam(OSC3_TRI_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 3 Triangle Level", "%", 0.f, 100.f);
        configParam(OSC1_WIDTH_PARAM, 0.01f, 0.99f, 0.5f, "Osc 1 Pulse Width", "%", 0.f, 100.f);
        configParam(OSC2_WIDTH_PARAM, 0.01f, 0.99f, 0.5f, "Osc 2 Pulse Width", "%", 0.f, 100.f);
        configParam(OSC3_WIDTH_PARAM, 0.01f, 0.99f, 0.5f, "Osc 3 Pulse Width", "%", 0.f, 100.f);
        configParam(OSC1_SAW_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 1 Saw Level", "%", 0.f, 100.f);
        configParam(OSC2_SAW_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 2 Saw Level", "%", 0.f, 100.f);
        configParam(OSC3_SAW_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 3 Saw Level", "%", 0.f, 100.f);
        configParam(OSC1_PWM_PARAM, 0.f, 1.f, 0.f, "Osc 1 PWM Amount", "%", 0.f, 100.f);
        configParam(OSC2_PWM_PARAM, 0.f, 1.f, 0.f, "Osc 2 PWM Amount", "%", 0.f, 100.f);
        configParam(OSC3_PWM_PARAM, 0.f, 1.f, 0.f, "Osc 3 PWM Amount", "%", 0.f, 100.f);
        configParam(OSC1_SQR_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 1 Square Level", "%", 0.f, 100.f);
        configParam(OSC2_SQR_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 2 Square Level", "%", 0.f, 100.f);
        configParam(OSC3_SQR_LEVEL_PARAM, 0.f, 1.f, 0.5f, "Osc 3 Square Level", "%", 0.f, 100.f);
        configParam(ENV_ATTACK_PARAM, 0.001f, 5.f, 0.1f, "Attack Time", " s", 0.f, 1.f);
        configParam(ENV_DECAY_PARAM, 0.001f, 5.f, 0.3f, "Decay Time", " s", 0.f, 1.f);
        configParam(LFO2_FREQ_PARAM, 0.01f, 10.f, 1.f, "LFO 2 Frequency", " Hz", 0.f, 1.f);
        configParam(LFO1_LEVEL_PARAM, 0.f, 1.f, 0.5f, "LFO 1 Level", "%", 0.f, 100.f);
        configParam(FILTER_CUTOFF_PARAM, 20.f, 20000.f, 1000.f, "Filter Cutoff", " Hz", 0.f, 1.f);
        configParam(ENV_SUSTAIN_PARAM, 0.f, 1.f, 0.5f, "Sustain Level", "%", 0.f, 100.f);
        configParam(AMP_LEVEL_PARAM, 0.f, 1.f, 0.8f, "Amplifier Level", "%", 0.f, 100.f);
        configParam(ENV_RELEASE_PARAM, 0.001f, 10.f, 1.f, "Release Time", " s", 0.f, 1.f);
        configParam(LFO1_FREQ_PARAM, 0.01f, 10.f, 0.5f, "LFO 1 Frequency", " Hz", 0.f, 1.f);
        configParam(LFO2_LEVEL_PARAM, 0.f, 1.f, 0.5f, "LFO 2 Level", "%", 0.f, 100.f);
        
        configInput(VOCT_INPUT, "V/Oct");
        configInput(AMP_CV_INPUT, "AMP CV");
        configInput(OSC1_PWM_CV_INPUT, "Osc 1 PWM CV");
        configInput(OSC2_PWM_CV_INPUT, "Osc 2 PWM CV");
        configInput(OSC3_PWM_CV_INPUT, "Osc 3 PWM CV");
        configInput(FILTER_CUT_CV_INPUT, "Filter Cutoff CV");
        configInput(ENV_GATE_INPUT, "Gate");
        
        configOutput(OSC1_TRI_OUTPUT, "Osc 1 Triangle");
        configOutput(OSC2_TRI_OUTPUT, "Osc 2 Triangle");
        configOutput(OSC3_TRI_OUTPUT, "Osc 3 Triangle");
        configOutput(OSC1_SAW_OUTPUT, "Osc 1 Saw");
        configOutput(OSC2_SAW_OUTPUT, "Osc 2 Saw");
        configOutput(OSC3_SAW_OUTPUT, "Osc 3 Saw");
        configOutput(OSC1_SQR_OUTPUT, "Osc 1 Square");
        configOutput(OSC2_SQR_OUTPUT, "Osc 2 Square");
        configOutput(OSC3_SQR_OUTPUT, "Osc 3 Square");
        configOutput(LFO2_OUT_OUTPUT, "LFO 2");
        configOutput(ENV_OUT_OUTPUT, "Envelope");
        configOutput(LFO1_OUT_OUTPUT, "LFO 1");
        configOutput(AMP_OUT_OUTPUT, "Output");
    }

    void process(const ProcessArgs& args) override {
        float deltaTime = args.sampleTime;
        filter.sampleRate = args.sampleRate;
        
        // Process LFOs
        lfo1.freq = params[LFO1_FREQ_PARAM].getValue();
        lfo2.freq = params[LFO2_FREQ_PARAM].getValue();
        
        lfo1.updatePhase(deltaTime);
        lfo2.updatePhase(deltaTime);
        
        float lfo1Out = lfo1.triangle() * params[LFO1_LEVEL_PARAM].getValue();
        float lfo2Out = lfo2.triangle() * params[LFO2_LEVEL_PARAM].getValue();
        
        // Set LFO outputs
        outputs[LFO1_OUT_OUTPUT].setVoltage(5.f * lfo1Out);
        outputs[LFO2_OUT_OUTPUT].setVoltage(5.f * lfo2Out);
        
        // Process envelope
        bool gate = inputs[ENV_GATE_INPUT].getVoltage() >= 1.f;
        
        env.attackTime = params[ENV_ATTACK_PARAM].getValue();
        env.decayTime = params[ENV_DECAY_PARAM].getValue();
        env.sustainLevel = params[ENV_SUSTAIN_PARAM].getValue();
        env.releaseTime = params[ENV_RELEASE_PARAM].getValue();
        
        env.process(deltaTime, gate);
        outputs[ENV_OUT_OUTPUT].setVoltage(10.f * env.output);
        
        // Process V/Oct input
        float pitch = 0.f;
        if (inputs[VOCT_INPUT].isConnected()) {
            pitch = inputs[VOCT_INPUT].getVoltage();
        }
        
        // Set oscillator parameters
        osc1.freq = dsp::FREQ_C4 * std::pow(2.f, pitch + params[OSC1_FREQ_PARAM].getValue());
        osc2.freq = dsp::FREQ_C4 * std::pow(2.f, pitch + params[OSC2_FREQ_PARAM].getValue());
        osc3.freq = dsp::FREQ_C4 * std::pow(2.f, pitch + params[OSC3_FREQ_PARAM].getValue());
        
        // Process PWM
        float osc1PwmAmount = params[OSC1_PWM_PARAM].getValue();
        float osc2PwmAmount = params[OSC2_PWM_PARAM].getValue();
        float osc3PwmAmount = params[OSC3_PWM_PARAM].getValue();
        
        float osc1PwmCV = inputs[OSC1_PWM_CV_INPUT].getVoltage() / 10.f;
        float osc2PwmCV = inputs[OSC2_PWM_CV_INPUT].getVoltage() / 10.f;
        float osc3PwmCV = inputs[OSC3_PWM_CV_INPUT].getVoltage() / 10.f;
        
        osc1.pw = clamp(params[OSC1_WIDTH_PARAM].getValue() + osc1PwmAmount * osc1PwmCV * 0.5f, 0.01f, 0.99f);
        osc2.pw = clamp(params[OSC2_WIDTH_PARAM].getValue() + osc2PwmAmount * osc2PwmCV * 0.5f, 0.01f, 0.99f);
        osc3.pw = clamp(params[OSC3_WIDTH_PARAM].getValue() + osc3PwmAmount * osc3PwmCV * 0.5f, 0.01f, 0.99f);
        
        // Update oscillator phases
        osc1.updatePhase(deltaTime);
        osc2.updatePhase(deltaTime);
        osc3.updatePhase(deltaTime);
        
        // Get oscillator outputs
        float osc1Tri = osc1.triangle() * params[OSC1_TRI_LEVEL_PARAM].getValue();
        float osc2Tri = osc2.triangle() * params[OSC2_TRI_LEVEL_PARAM].getValue();
        float osc3Tri = osc3.triangle() * params[OSC3_TRI_LEVEL_PARAM].getValue();
        
        float osc1Saw = osc1.saw() * params[OSC1_SAW_LEVEL_PARAM].getValue();
        float osc2Saw = osc2.saw() * params[OSC2_SAW_LEVEL_PARAM].getValue();
        float osc3Saw = osc3.saw() * params[OSC3_SAW_LEVEL_PARAM].getValue();
        
        float osc1Sqr = osc1.square() * params[OSC1_SQR_LEVEL_PARAM].getValue();
        float osc2Sqr = osc2.square() * params[OSC2_SQR_LEVEL_PARAM].getValue();
        float osc3Sqr = osc3.square() * params[OSC3_SQR_LEVEL_PARAM].getValue();
        
        // Set individual oscillator outputs
        outputs[OSC1_TRI_OUTPUT].setVoltage(5.f * osc1Tri);
        outputs[OSC2_TRI_OUTPUT].setVoltage(5.f * osc2Tri);
        outputs[OSC3_TRI_OUTPUT].setVoltage(5.f * osc3Tri);
        
        outputs[OSC1_SAW_OUTPUT].setVoltage(5.f * osc1Saw);
        outputs[OSC2_SAW_OUTPUT].setVoltage(5.f * osc2Saw);
        outputs[OSC3_SAW_OUTPUT].setVoltage(5.f * osc3Saw);
        
        outputs[OSC1_SQR_OUTPUT].setVoltage(5.f * osc1Sqr);
        outputs[OSC2_SQR_OUTPUT].setVoltage(5.f * osc2Sqr);
        outputs[OSC3_SQR_OUTPUT].setVoltage(5.f * osc3Sqr);
        
        // Mix all oscillator outputs
        float mixedOutput = (osc1Tri + osc1Saw + osc1Sqr + 
                            osc2Tri + osc2Saw + osc2Sqr + 
                            osc3Tri + osc3Saw + osc3Sqr) / 9.f; // Normalize the mix
        
        // Process filter
        float cutoffBase = params[FILTER_CUTOFF_PARAM].getValue();
        float cutoffCV = inputs[FILTER_CUT_CV_INPUT].getVoltage() / 10.f; // Normalize to 0-1 range
        
        // Exponential cutoff control with CV - cap at 20kHz
        filter.cutoff = clamp(cutoffBase * pow(2.f, cutoffCV * 10.f), 20.f, 20000.f);
        filter.process(mixedOutput);
        
        // Process VCA
        float vcaGain = params[AMP_LEVEL_PARAM].getValue();
        float vcaCV = inputs[AMP_CV_INPUT].getVoltage() / 10.f; // Normalize to 0-1 range
        
        float finalOutput = 0.f;
        // Final output stage with envelope modulation
        if (inputs[ENV_GATE_INPUT].isConnected()) {
            finalOutput = filter.output * vcaGain * (1.f + vcaCV) * env.output;
        else {
            finalOutput = filter.output * vcaGain * (1.f + vcaCV);
        }
        
        // Set VCA output
        outputs[AMP_OUT_OUTPUT].setVoltage(5.f * finalOutput);
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

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 16.3)), module, Sub_osc::OSC1_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 16.3)), module, Sub_osc::OSC2_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(127.0, 16.3)), module, Sub_osc::OSC3_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 24.3)), module, Sub_osc::OSC1_TRI_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 24.3)), module, Sub_osc::OSC2_TRI_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(153.0, 24.3)), module, Sub_osc::OSC3_TRI_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 31.3)), module, Sub_osc::OSC1_WIDTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 31.3)), module, Sub_osc::OSC2_WIDTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(127.0, 31.3)), module, Sub_osc::OSC3_WIDTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 39.3)), module, Sub_osc::OSC1_SAW_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 39.3)), module, Sub_osc::OSC2_SAW_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(153.0, 39.3)), module, Sub_osc::OSC3_SAW_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 46.3)), module, Sub_osc::OSC1_PWM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 46.3)), module, Sub_osc::OSC2_PWM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(127.0, 46.3)), module, Sub_osc::OSC3_PWM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 54.3)), module, Sub_osc::OSC1_SQR_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 54.3)), module, Sub_osc::OSC2_SQR_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(153.0, 54.3)), module, Sub_osc::OSC3_SQR_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 66.3)), module, Sub_osc::ENV_ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 81.3)), module, Sub_osc::ENV_DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 81.3)), module, Sub_osc::LFO2_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 81.3)), module, Sub_osc::LFO1_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 81.3)), module, Sub_osc::FILTER_CUTOFF_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 96.3)), module, Sub_osc::ENV_SUSTAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(101.0, 96.3)), module, Sub_osc::AMP_LEVEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.0, 111.3)), module, Sub_osc::ENV_RELEASE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(49.0, 111.3)), module, Sub_osc::LFO1_FREQ_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.0, 111.3)), module, Sub_osc::LFO2_LEVEL_PARAM));
		
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.0, 16.3)), module, Sub_osc::VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.0, 46.3)), module, Sub_osc::OSC1_PWM_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.0, 46.3)), module, Sub_osc::OSC2_PWM_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.0, 46.3)), module, Sub_osc::OSC3_PWM_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(88.0, 81.3)), module, Sub_osc::FILTER_CUT_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.0, 111.3)), module, Sub_osc::ENV_GATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(88.0, 96.3)), module, Sub_osc::AMP_CV_INPUT));
		
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 24.3)), module, Sub_osc::OSC1_TRI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.0, 24.3)), module, Sub_osc::OSC2_TRI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 24.3)), module, Sub_osc::OSC3_TRI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 39.3)), module, Sub_osc::OSC1_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.0, 39.3)), module, Sub_osc::OSC2_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 39.3)), module, Sub_osc::OSC3_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 54.3)), module, Sub_osc::OSC1_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.0, 54.3)), module, Sub_osc::OSC2_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(140.0, 54.3)), module, Sub_osc::OSC3_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.0, 81.3)), module, Sub_osc::LFO2_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.0, 111.3)), module, Sub_osc::ENV_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.0, 111.3)), module, Sub_osc::LFO1_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(101.0, 111.3)), module, Sub_osc::AMP_OUT_OUTPUT));
	}
};


Model* modelSub_osc = createModel<Sub_osc, Sub_oscWidget>("sub_osc");