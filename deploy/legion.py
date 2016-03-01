from fabric.api import *
from mako.template import Template
import mako
import os
from contextlib import nested

env.run_at="/home/ucgajhe/Scratch/Smooth/output"
env.deploy_to="/home/ucgajhe/devel/smooth"
env.clone_url="https://github.com/UCL/SmoothLifeExample.git"
env.hosts=['legion.rc.ucl.ac.uk']
env.user='ucgajhe'

modules = nested(
    prefix('module load cmake'),
    prefix('module swap compilers compilers/gnu/4.9.2'),
    prefix('module swap mpi mpi/openmpi/1.10.1/gnu-4.9.2')
)

@task
def cold(branch='mpi'):
    run('rm -rf '+env.deploy_to)
    run('mkdir -p '+env.deploy_to)
    run('mkdir -p '+env.run_at)
    with cd(env.deploy_to):
        with modules:
            run('git clone '+env.clone_url)
            run('mkdir SmoothLifeExample/build')
            with cd('SmoothLifeExample/build'):
                run('git checkout '+branch)
                run('cmake .. -DCMAKE_CXX_COMPILER=mpiCC -DCMAKE_C_COMPILER=mpicc')
                run('make')
                run('test/catch')

@task
def warm(branch='mpi'):
  with cd(env.deploy_to+'/SmoothLifeExample/build'):
        with modules:
            run('git checkout '+branch)
            run('git pull')
            run('cmake ..')
            run('make')
            run('test/catch')

@task
def sub(processes=4):
    env.processes=processes
    template_file_path=os.path.join(os.path.dirname(__file__),'legion.sh.mko')
    script_local_path=os.path.join(os.path.dirname(__file__),'legion.sh')
    config_file_path=os.path.join(os.path.dirname(os.path.dirname(__file__)),'config.yml')
    with open(template_file_path) as template:
        script=Template(template.read()).render(**env)
        with open(script_local_path,'w') as script_file:
            script_file.write(script)
    with cd(env.run_at):
       put(config_file_path,'config.yml')
    with cd(env.deploy_to):
        put(script_local_path,'smooth.sh')
        run('qsub smooth.sh')

@task
def stat():
    run('qstat')

@task
def fetch():
    with lcd(os.path.join(os.path.dirname(os.path.dirname(__file__)),'results')):
      with cd(env.run_at):
        get('*')
