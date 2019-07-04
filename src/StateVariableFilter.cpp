#include "plugin.hpp"


struct StateVariableFilter : Module {
	enum ParamIds {
		CUTOFF_PARAM,
		RES_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	//state of filter
	float low;
	float cutoff;
	float res;
	float sampleRate;


	StateVariableFilter() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(CUTOFF_PARAM, 0.f, 1.f, 0.5f, "Frequency", " Hz", std::pow(2, 10.f), dsp::FREQ_C4 / std::pow(2, 5.f));
		configParam(RES_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs &args) override {
		if (!outputs[AUDIO_OUTPUT].isConnected()) {
			return;
		}

	    int channels = std::max(1, inputs[AUDIO_INPUT].getChannels());
	    float in[16] = {};
	    float out[16] = {};
	    Input audioinput = inputs[AUDIO_INPUT];

	    audioinput.readVoltages(in);


		cutoff = params[CUTOFF_PARAM].getValue();
		res = params[RES_PARAM].getValue();
		printf("Cutoff: %f\n", cutoff);


		//doesn't seem to capture actual sample rate. but will use it anyway
		sampleRate = (1.0/args.sampleTime);

		//can increase to more channels
		out[0] = in[0];

		// simple frequency tuning with error towards nyquist  
		// F is the filter's center frequency, and Fs is the sampling rate  
		//F1 = 2*pi*F/Fs 

		Output &audiooutput = outputs[AUDIO_OUTPUT];
		audiooutput.setChannels(channels);
		audiooutput.writeVoltages(out);
		low = out[0];
		printf("low: %f\n", low);
		
		
		
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
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.727, 52.7)), module, StateVariableFilter::RES_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(16.399, 84.506)), module, StateVariableFilter::AUDIO_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(34.039, 83.704)), module, StateVariableFilter::AUDIO_OUTPUT));
	}
};


Model *modelStateVariableFilter = createModel<StateVariableFilter, StateVariableFilterWidget>("StateVariableFilter");