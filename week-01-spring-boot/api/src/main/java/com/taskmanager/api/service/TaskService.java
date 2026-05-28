package com.taskmanager.api.service;

import com.taskmanager.api.model.Task;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class TaskService {
    private List<Task> tasks = new ArrayList<>();

    public TaskService() {
        tasks.add(new Task( "Master Spring Boot-1", "Learn annotations and MVC architecture", Task.Status.DONE));
        tasks.add(new Task( "Master Spring Boot-2", "Learn annotations and MVC architecture", Task.Status.IN_PROGRESS));
        tasks.add(new Task( "Review", "Review what I have learned", Task.Status.TODO));
        tasks.add(new Task( "Submit", "Send the project to friends", Task.Status.SUSPENDED));
    }

    public List<Task> getAllTasks(){
        return tasks;
    }

    public List<Task> getTaskByTitle(String title){
        List<Task> matchingTasks = new ArrayList<>();
        for(Task task : tasks){
            if (title.equalsIgnoreCase( task.getTitle()) ){
                matchingTasks.add(task);
            }
        }
        return matchingTasks;
    }
    public Task addTask(Task task){
        tasks.add(task);
        return task;
    }
    public boolean deleteTask(String title){
        return  tasks.removeIf(task -> title.equalsIgnoreCase(task.getTitle()));
    }
}
