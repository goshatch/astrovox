# A synth from scratch

This is an experimental project aimed at playing with sound generation. The target is to write an approximate software emulation of the Roland [Juno-60](https://en.wikipedia.org/wiki/Roland_Juno-60) synthesizer.

## Play project

This project is a “play project”: sound generation is a brand new domain to me, I’ve last worked with C in uni in 2005, so I have no hopes of a “good enough” outcome — only of exploring and enjoying the process.

## Components

My current understanding is that a synthesizer works like this:

```
┌───────────┐
│    DCO    │
└─────┬─────┘
      │
┌─────▼─────┐    ┌──────────┐
│  Filters  ◄────┤   LFOs   │
└─────┬─────┘    └─────┬────┘
      │                │
┌─────▼─────┐          │
│ Envelopes ◄──────────┘
└─────┬─────┘
      │
┌─────▼─────┐
│  Outputs  │
└───────────┘
```

### Oscillator (DCO)

### Filters

### Envelopes

### LFOs

### Audio output

## Contributing

Patches and comments are welcome. Please direct both to my [Public inbox](https://lists.sr.ht/~gosha/public-inbox) on Sourcehut.

## License

MIT License

Copyright (c) 2023 Gosha Tcherednitchenko

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next paragraph) shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
