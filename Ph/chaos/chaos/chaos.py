"""
module containing classes/functions for studies in chaos
"""
#!/usr/bin/env python
# implementation of an N-dimensional discrete map object
import matplotlib.pyplot as plt
import numpy as np

def get_fig_axes(title=None, xlabel=None, ylabel=None):
    """ get figure/axes to save code"""
    fig = plt.figure()
    axes = fig.add_subplot(1, 1, 1)
    if title is not None:
        axes.set_title(title)
    if xlabel is not None:
        axes.set_xlabel(xlabel)
    if ylabel is not None:
        axes.set_ylabel(ylabel)
    return fig, axes

class MathMap(object):
    """
    Represents a convenient container for an n-Dimensional map
    """
    def __init__(self,
                 init,
                 iter_func,
                 max_iter=1000):
        """
        Class variables:
            iter_func(np.array) -> np.array: iteration function
            next_iter int: next iteration (next index to write to)
            max_iter int: number of iterations to evolve for (default: 1000)
            state: n-dimensional np.array, initial conditions
        Args:
            init: state[0]
            iter_func: iter_func
            max_iter: max_iter
        """
        self.iter_func = iter_func
        self.curr_iter = 1
        self.max_iter = max_iter
        self.state = np.zeros([max_iter, init.size])
        self.state[0] = init

    @staticmethod
    def __finish(write_file, fig=plt):
        """
        Helper function to save/plot file and clear figure (repeated code)
        """
        if write_file is not None:
            fig.savefig(write_file)
        else:
            plt.show()
        plt.clf()

    def step(self):
        """
        Applies iter_func to current state and increments state
        """
        self.state[self.curr_iter] =\
                self.iter_func(self.state[self.curr_iter - 1])
        self.curr_iter += 1

    def run(self):
        """
        Call step() until curr_iter = max_iter

        returns self.state
        """
        while self.curr_iter < self.max_iter:
            self.step()
        return self.state

    def scatter(self,
                dim1=0,
                dim2=1,
                title=None,
                xlabel=None,
                ylabel=None,
                write_file=None):
        """
        For 1D, plots time series, else plots scatter plot along dim1, dim2
        Args:
            dim1: dimension to plot along x-axis (default: 0)
            dim2: dimension to plot along y axis (default: 1)
            write_file: if not None, filename to save file (default: None)
        """
        self.run() # ensure that simulation is done running
        if self.state.shape[1] == 1:
            xseries = np.arange(self.max_iter)
            yseries = self.state[ :, 0]
        else:
            xseries = self.state[ :, dim1]
            yseries = self.state[ :, dim2]

        fig, axes = get_fig_axes(title, xlabel, ylabel)
        axes.scatter(xseries, yseries)
        self.__finish(write_file, fig)

    def hist(self,
             dim=0,
             num_bins=30,
             title=None,
             xlabel=None,
             ylabel=None,
             write_file=None):
        """
        Histogram self.state[ :, dim] w/ num_bins total bins
        Args:
            dim: which dimension to plot (default: 0)
            num_bins: numbeer of bins to use (default: 30)
            write_file: if not None, filename to save file (default: None)
        """
        self.run() # ensure that simulation is done running
        fig, axes = get_fig_axes(title, xlabel, ylabel)
        axes.hist(self.state[ :, dim], bins=num_bins)
        self.__finish(write_file, fig)

    def plot(self,
             dim=0,
             title=None,
             xlabel=None,
             ylabel=None,
             write_file=None):
        """
        Plot time trajectory of a singel dimension
        Args:
            dim: which dimension to plot (default: 0)
            write_file: if not None, filename to save file (default: None)
        """
        self.run() # ensure that simulation is done running
        fig, axes = get_fig_axes(title, xlabel, ylabel)
        axes.plot(self.state[ :, dim])
        self.__finish(write_file, fig)
