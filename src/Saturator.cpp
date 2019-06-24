#include "plugin.hpp"


struct Saturator : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		PATH17_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		PATH19_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Saturator() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs &args) override {
		Input &in = inputs[PATH17_INPUT];
		int channels = in.getChannels();
		simd::float_4 v[4];
		for (int c = 0; c < channels; c += 4) {
			v[c / 4] = simd::float_4::load(in.getVoltages(c));
		}


		Output &out = outputs[PATH19_OUTPUT];
		out.setChannels(channels);
		for (int c = 0; c < channels; c += 4) {
			v[c / 4].store(out.getVoltages(c));
		}

	}
};


struct SaturatorWidget : ModuleWidget {
	SaturatorWidget(Saturator *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Saturator.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(19.874, 26.241)), module, Saturator::PATH17_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.874, 74.617)), module, Saturator::PATH19_OUTPUT));
	}
};


Model *modelSaturator = createModel<Saturator, SaturatorWidget>("Saturator");