/* Copyright (c) 2011 by Felix Lauer and Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

# ifndef ANIMATEDVALUE_HPP
# define ANIMATEDVALUE_HPP

class AnimatedValue {
    public:
        enum Direction { In, Out, InOut, OutIn, Linear };

        AnimatedValue();
        AnimatedValue(Direction direction, double start, double end, double duration, double multiplier = 0);

        void resetTarget(double end, double duration);
        void update(double time);

        double val() const;
        double start() const;
        double end() const;

    private:
        double updateLinear(double t, double s, double e);
        double updateEaseIn(double t, double s, double e);
        double updateEaseOut(double t, double s, double e);
        double updateEaseInOut(double t, double s, double e);
        double updateEaseOutIn(double t, double s, double e);

        Direction direction_;

        double val_, start_;
        double end_, state_;
        double duration_, multiplier_;

};

# endif //ANIMATEDVALUE_HPP

