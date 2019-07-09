#include "plugin.hpp"


struct StateVariableFilter : Module {
	enum ParamIds {
		CUTOFF_PARAM,
		DAMP_PARAM,
		DRIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		LOW_OUTPUT,
		HIGH_OUTPUT,
		BAND_OUTPUT,
		NOTCH_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	//state of filter
	float lowpass, highpass, bandpass, notch;
	float freq; //center frequency based on cutoff
	float cutoff;
	float damp;
	float drive;
	float sampleRate;


	StateVariableFilter() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(CUTOFF_PARAM, 0.f, 1.f, 0.5f, "Frequency", " Hz", std::pow(2, 10.f), dsp::FREQ_C4 / std::pow(2, 5.f));
		configParam(DAMP_PARAM, 0.f, 1.f, 0.f, "Damping");
		configParam(DRIVE_PARAM,0.f,1.f,0.f,"Drive","",0,10);
		lowpass=highpass=bandpass=notch=0;
	}

	void process(const ProcessArgs &args) override {
		if (!(outputs[LOW_OUTPUT].isConnected()  || 
			  outputs[HIGH_OUTPUT].isConnected() ||
			  outputs[BAND_OUTPUT].isConnected() ||
			  outputs[NOTCH_OUTPUT].isConnected())) {
			return;
		}


		//get input sample and params
	    Input &audioInput = inputs[AUDIO_INPUT];
	    float input = audioInput.getVoltage();
	    float cutoffParam = params[CUTOFF_PARAM].getValue() * 10.f - 5.f;
		float dampParam = params[DAMP_PARAM].getValue();
		float driveParam = params[DRIVE_PARAM].getValue();
		sampleRate = (1.0/args.sampleTime);

		//set state
		float gain = pow(1.f + driveParam, 5); //gain from drive
		cutoff = dsp::FREQ_C4 * pow(2.f, cutoffParam);
		damp = 1 - clamp(dampParam,0.1f,1.f); //invert damping param just for intuition
		freq = 2*M_PI*cutoff/sampleRate; //set center frequency
		//modify input signal
		input += 1e-5f * (2.f * random::uniform() - 1.f);
		input *= gain;


		//SVF model
		lowpass = lowpass+freq*bandpass;
		highpass = damp*input - lowpass - damp*bandpass;
		bandpass = freq * highpass + bandpass;
		notch = highpass+lowpass;

		//set outs
		outputs[LOW_OUTPUT].setVoltage(5.0*lowpass);
		outputs[HIGH_OUTPUT].setVoltage(5.0*highpass);
		outputs[BAND_OUTPUT].setVoltage(5.0*highpass);
		outputs[NOTCH_OUTPUT].setVoltage(5.0*notch);
		//audiooutput.setChannels(channels);
		//audiooutput.writeVoltages(out);
		//low = out[0];
		//printf("low: %f\n", low);
		
		
		
	}
};


struct StateVariableFilterWidget : ModuleWidget {
	StateVariableFilterWidget(StateVariableFilter *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/StateVariableFilter.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.326, 23.702)), module, StateVariableFilter::CUTOFF_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.727, 52.7)), module, StateVariableFilter::DAMP_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.727, 62.7)), module, StateVariableFilter::DRIVE_PARAM));


		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10, 20)), module, StateVariableFilter::AUDIO_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15, 85)), module, StateVariableFilter::LOW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35, 85)), module, StateVariableFilter::HIGH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15, 105)), module, StateVariableFilter::BAND_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35, 105)), module, StateVariableFilter::NOTCH_OUTPUT));



	}
};


Model *modelStateVariableFilter = createModel<StateVariableFilter, StateVariableFilterWidget>("StateVariableFilter");